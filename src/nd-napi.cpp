// Netify Agent
// Copyright (C) 2015-2024 eGloo Incorporated
// <http://www.egloo.ca>
//
// This program is free software: you can redistribute it
// and/or modify it under the terms of the GNU General
// Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fstream>

#include "nd-config.hpp"
#include "nd-except.hpp"
#include "nd-napi.hpp"
#include "nd-napi-curl.hpp"
#include "nd-sha1.h"
#include "nd-util.hpp"

using namespace std;
using json = nlohmann::json;

static int ndNetifyApiThread_curl_debug(CURL *ch
  __attribute__((unused)),
  curl_infotype type, char *data, size_t size, void *param) {
    if (! ndGC_DEBUG_CURL) return 0;

    string buffer;
    ndThread *thread = reinterpret_cast<ndThread *>(param);

    switch (type) {
    case CURLINFO_TEXT:
        buffer.assign(data, size);
        nd_dprintf("%s: %s", thread->GetTag().c_str(),
          buffer.c_str());
        break;
    case CURLINFO_HEADER_IN:
        buffer.assign(data, size);
        nd_dprintf("%s: <-- %s", thread->GetTag().c_str(),
          buffer.c_str());
        break;
    case CURLINFO_HEADER_OUT:
        buffer.assign(data, size);
        nd_dprintf("%s: --> %s", thread->GetTag().c_str(),
          buffer.c_str());
        break;
    case CURLINFO_DATA_IN:
        nd_dprintf("%s: <-- %lu data bytes\n",
          thread->GetTag().c_str(), size);
        break;
    case CURLINFO_DATA_OUT:
        nd_dprintf("%s: --> %lu data bytes\n",
          thread->GetTag().c_str(), size);
        break;
    case CURLINFO_SSL_DATA_IN:
        nd_dprintf("%s: <-- %lu SSL bytes\n",
          thread->GetTag().c_str(), size);
        break;
    case CURLINFO_SSL_DATA_OUT:
        nd_dprintf("%s: --> %lu SSL bytes\n",
          thread->GetTag().c_str(), size);
        break;
    default: break;
    }

    return 0;
}

static size_t ndNetifyApiThread_read_data(char *data,
  size_t size, size_t nmemb, void *user) {
    size_t length = size * nmemb;
    ndNetifyApiThread *thread_napi =
      reinterpret_cast<ndNetifyApiThread *>(user);

    thread_napi->AppendContent((const char *)data, length);

    return length;
}

static size_t ndNetifyApiThread_parse_header(char *data,
  size_t size, size_t nmemb, void *user) {
    size_t length = size * nmemb;

    if (size != 1 || length == 0) return 0;

    ndNetifyApiThread *thread_napi =
      reinterpret_cast<ndNetifyApiThread *>(user);

    string header_data;
    header_data.assign(data, length);

    thread_napi->ParseHeader(header_data);

    return length;
}

#if (LIBCURL_VERSION_NUM < 0x073200)
static int ndNetifyApiThread_progress(void *user,
  double dltotal __attribute__((unused)),
  double dlnow __attribute__((unused)),
  double ultotal __attribute__((unused)),
  double ulnow __attribute__((unused)))
#else
static int ndNetifyApiThread_progress(void *user,
  curl_off_t dltotal __attribute__((unused)),
  curl_off_t dlnow __attribute__((unused)),
  curl_off_t ultotal __attribute__((unused)),
  curl_off_t ulnow __attribute__((unused)))
#endif
{
    ndNetifyApiThread *thread_napi =
      reinterpret_cast<ndNetifyApiThread *>(user);

    if (thread_napi->ShouldTerminate()) return 1;

    return 0;
}

ndNetifyApiCurl::ndNetifyApiCurl() {
    if ((ch = curl_easy_init()) == nullptr) {
        throw ndException(__PRETTY_FUNCTION__,
          "curl_easy_init");
    }

    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 3);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(ch, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 60);
    curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, ndGC.napi_tls_verify);
    curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, (ndGC.napi_tls_verify) ? 2 : 0);
    // curl_easy_setopt(ch, CURLOPT_SSLVERSION,
    // CURL_SSLVERSION_TLSv1);

#ifdef _ND_WITH_LIBCURL_ZLIB
#if (LIBCURL_VERSION_NUM >= 0x072106)
    curl_easy_setopt(ch, CURLOPT_ACCEPT_ENCODING, "gzip");
#endif
#endif  // _ND_WITH_LIBCURL_ZLIB
}

ndNetifyApiCurl::~ndNetifyApiCurl() {
    if (ch != nullptr) {
        curl_easy_cleanup(ch);
        ch = nullptr;
    }
}

ndNetifyApiThread::ndNetifyApiThread(const string &tag)
  : ndThread(tag) {
    curl_private = static_cast<void *>(new ndNetifyApiCurl);

    curl_easy_setopt(ndCurl->ch, CURLOPT_WRITEFUNCTION,
      ndNetifyApiThread_read_data);
    curl_easy_setopt(ndCurl->ch, CURLOPT_WRITEDATA,
      static_cast<void *>(this));

    curl_easy_setopt(ndCurl->ch, CURLOPT_HEADERFUNCTION,
      ndNetifyApiThread_parse_header);
    curl_easy_setopt(ndCurl->ch, CURLOPT_HEADERDATA,
      static_cast<void *>(this));

    curl_easy_setopt(ndCurl->ch, CURLOPT_NOPROGRESS, 0);
#if (LIBCURL_VERSION_NUM < 0x073200)
    curl_easy_setopt(ndCurl->ch, CURLOPT_PROGRESSFUNCTION,
      ndNetifyApiThread_progress);
    curl_easy_setopt(ndCurl->ch, CURLOPT_PROGRESSDATA,
      static_cast<void *>(this));
#else
    curl_easy_setopt(ndCurl->ch, CURLOPT_XFERINFOFUNCTION,
      ndNetifyApiThread_progress);
    curl_easy_setopt(ndCurl->ch, CURLOPT_XFERINFODATA,
      static_cast<void *>(this));
#endif
    if (ndGC_DEBUG_CURL) {
        curl_easy_setopt(ndCurl->ch, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(ndCurl->ch, CURLOPT_DEBUGFUNCTION,
          ndNetifyApiThread_curl_debug);
        curl_easy_setopt(ndCurl->ch, CURLOPT_DEBUGDATA,
          static_cast<void *>(this));
    }
}

ndNetifyApiThread::~ndNetifyApiThread() {

    Terminate();
    Join();

    DestroyHeaders();

    if (curl_private != nullptr) {
        delete ndCurl;
        curl_private = nullptr;
    }
}

void ndNetifyApiThread::AppendContent(const char *data,
  size_t length) {
    if (content_filename.empty())
        content.append(data, length);
    else {
        ofstream ofs(content_filename, ofstream::app);
        if (! ofs.is_open()) {
            throw ndException("%s: %s: %s", __PRETTY_FUNCTION__,
              content_filename.c_str(), strerror(EINVAL));
        }

        string buffer;
        buffer.assign(data, length);
        ofs << buffer;
    }
}

void ndNetifyApiThread::ParseHeader(const string &header_raw) {
    string key, value;
    size_t p = string::npos;
    if ((p = header_raw.find_first_of(":")) != string::npos) {
        key = header_raw.substr(0, p);
        value = header_raw.substr(p + 1);
    }

    if (! key.empty() && ! value.empty()) {
        transform(key.begin(), key.end(), key.begin(),
          [](unsigned char c) { return tolower(c); });

        nd_trim(key);
        nd_trim(value);

        if (headers_rx.find(key) == headers_rx.end()) {
            headers_rx[key] = value;
            if (ndGC_DEBUG_CURL) {
                nd_dprintf("%s: header: %s: %s\n",
                  tag.c_str(), key.c_str(), value.c_str());
            }
        }
    }
}

void ndNetifyApiThread::CreateHeaders(const Headers &headers) {
    DestroyHeaders();

    string header("User-Agent: ");
    header.append(nd_get_version_and_features());
    ndCurl->headers_tx = curl_slist_append(ndCurl->headers_tx, header.c_str());

    header = "X-Vendor-ID: ";
    header.append(ndGC.napi_vendor.c_str());
    ndCurl->headers_tx = curl_slist_append(ndCurl->headers_tx, header.c_str());

    for (auto &h : headers) {
        header = h.first;
        header.append(": ");
        header.append(h.second);
        ndCurl->headers_tx = curl_slist_append(ndCurl->headers_tx, header.c_str());
    }

    curl_easy_setopt(ndCurl->ch, CURLOPT_HTTPHEADER, ndCurl->headers_tx);
}

void ndNetifyApiThread::DestroyHeaders(void) {
    if (ndCurl->headers_tx != nullptr) {
        curl_slist_free_all(ndCurl->headers_tx);
        ndCurl->headers_tx = nullptr;
    }
}

void ndNetifyApiThread::Perform(Method method, const string &url,
  const Headers &headers, const string &payload) {
    ndCurl->curl_rc = CURLE_OK;
    curl_easy_setopt(ndCurl->ch, CURLOPT_URL, url.c_str());

    ndCurl->http_rc = -1;
    content.clear();
    headers_rx.clear();

    CreateHeaders(headers);

    curl_easy_setopt(ndCurl->ch, CURLOPT_NOBODY, 0);

    switch (method) {
    case Method::GET:
        curl_easy_setopt(ndCurl->ch, CURLOPT_POST, 0);
        nd_dprintf("%s: %s: %s\n", tag.c_str(), "GET", url.c_str());
        break;
    case Method::HEAD:
        curl_easy_setopt(ndCurl->ch, CURLOPT_POST, 0);
        curl_easy_setopt(ndCurl->ch, CURLOPT_NOBODY, 1);

        nd_dprintf("%s: %s: %s\n", tag.c_str(), "HEAD",
          url.c_str());
        break;
    case Method::POST:
        curl_easy_setopt(ndCurl->ch, CURLOPT_POST, 1);
        curl_easy_setopt(ndCurl->ch, CURLOPT_POSTFIELDSIZE, payload.size());
        if (payload.size()) {
            curl_easy_setopt(ndCurl->ch, CURLOPT_POSTFIELDS,
              payload.c_str());
        }

        nd_dprintf("%s: %s: %s: %u byte(s)\n", tag.c_str(),
          "POST", url.c_str(), payload.size());
        break;
    }

    if ((ndCurl->curl_rc = curl_easy_perform(ndCurl->ch)) != CURLE_OK)
        throw ndCurl->curl_rc;

    if ((ndCurl->curl_rc = curl_easy_getinfo(ndCurl->ch,
           CURLINFO_RESPONSE_CODE, &ndCurl->http_rc)) != CURLE_OK)
        throw ndCurl->curl_rc;

    char *ct = nullptr;
    curl_easy_getinfo(ndCurl->ch, CURLINFO_CONTENT_TYPE, &ct);
    if (ct != nullptr) content_type = ct;
    else {
        auto i = headers_rx.find("content-type");
        if (i == headers_rx.end()) content_type.clear();
        else {
            content_type = i->second;
        }
    }
}

void *ndNetifyApiBootstrap::Entry(void) {

    static map<ndUUID, string> uuids = {
        { ndUUID::AGENT, "X-UUID" },
        { ndUUID::SERIAL, "X-UUID-Serial" },
        { ndUUID::SITE, "X-UUID-Site" }
    };

    Headers headers;

    for (auto &uuid : uuids) {
        string value("-");

        if (! ndGC.LoadUUID(uuid.first, value)) {
            nd_dprintf("%s: no UUID set for: %s\n",
              tag.c_str(), uuid.second.c_str());
        }
        headers.insert(make_pair(uuid.second, value));
    }

    string url = ndGC.url_napi_bootstrap;

    try {
        Perform(ndNetifyApiThread::Method::POST, url, headers);
    }
    catch (CURLcode &rc) {
        nd_dprintf("%s: bootstrap request error: %s\n",
          tag.c_str(), curl_easy_strerror(rc));
    }

    return nullptr;
}

ndNetifyApiDownload::ndNetifyApiDownload(const string &token,
  const string &url, const string &filename)
  : ndNetifyApiThread("api-download"), token(token),
    url(url) {
    if (! filename.empty()) {
        nd_sha1_file(filename, digest);
        nd_basename(filename, tag);
    }
}

ndNetifyApiDownload::~ndNetifyApiDownload() {
    if (! content_filename.empty())
        unlink(content_filename.c_str());
}

void *ndNetifyApiDownload::Entry(void) {
    string bearer("Bearer ");
    bearer.append(token);

    Headers headers;
    headers.insert(make_pair("Authorization", bearer));

    nd_tmpfile("/tmp/nd-napi", content_filename);

    try {
        Perform(ndNetifyApiThread::Method::HEAD, url, headers);

        if (ndCurl->http_rc == 200) {
            auto hdr_sha1 = headers_rx.find("x-sha1-hash");
            if (hdr_sha1 == headers_rx.end()) {
                nd_dprintf(
                  "%s: no SHA1 hash found in headers, "
                  "can't compare.\n",
                  tag.c_str());

                Perform(ndNetifyApiThread::Method::GET, url, headers);
            }
            else {
                string old_hash;
                nd_sha1_to_string(digest, old_hash);

                if (old_hash == hdr_sha1->second) {
                    nd_dprintf(
                      "%s: file has not changed.\n", tag.c_str());
                    ndCurl->http_rc = 304;
                    content =
                      "{\"status_code\":304, "
                      "\"status_message\":\"Not "
                      "modified\"}";
                }
                else {
                    nd_dprintf(
                      "%s: file has changed, downloading "
                      "update...\n",
                      tag.c_str());
                    Perform(ndNetifyApiThread::Method::GET,
                      url, headers);
                }
            }
        }
    }
    catch (CURLcode &rc) {
        nd_dprintf("%s: download request error: %s\n",
          tag.c_str(), curl_easy_strerror(rc));
    }

    return nullptr;
}

ndNetifyApiManager::UpdateResult ndNetifyApiManager::Update(void) {
    UpdateResult result = UpdateResult::OK;
    auto request = requests.find(Request::BOOTSTRAP);

    if (request != requests.end()) {
        ndNetifyApiBootstrap *bootstrap =
          static_cast<ndNetifyApiBootstrap *>(request->second);

        if (bootstrap->HasTerminated()) {
            result = ProcessBootstrapRequest(bootstrap);

            requests.erase(request);
            delete bootstrap;
        }
    }
    else {
        ndNetifyApiBootstrap *bootstrap = new ndNetifyApiBootstrap();

        auto request = requests.insert(
          make_pair(Request::BOOTSTRAP, bootstrap));

        if (request.second != true) delete bootstrap;
        else {
            try {
                request.first->second->Create();
            }
            catch (exception &e) {
                nd_printf(
                  "netify-api: Error creating bootstrap "
                  "request: %s\n",
                  e.what());
                delete bootstrap;
                requests.erase(request.first);
            }
        }
    }

    size_t downloads = 0;
    static vector<Request> types = {
        Request::DOWNLOAD_APPLICATIONS,
        Request::DOWNLOAD_APPLICATIONS_DAT,
        Request::DOWNLOAD_CATEGORIES,
        Request::DOWNLOAD_INTELLIGENCE,
        Request::DOWNLOAD_INTELLIGENCE_DAT,
    };

    for (auto &type : types) {
        auto request = requests.find(type);

        if (request != requests.end()) {
            downloads++;
            ndNetifyApiDownload *download =
              static_cast<ndNetifyApiDownload *>(request->second);

            if (download->HasTerminated()) {
                download_results[type] =
                  ProcessDownloadRequest(download, type);

                requests.erase(request);
                delete download;
            }
        }
    }

    if (downloads == 0 && download_results.size()) {
        bool reload = false;
        size_t successful = 0;

        for (auto &r : download_results) {
            if (! r.second) continue;
            reload = true;
            successful++;
        }

        nd_dprintf(
          "netify-api: %lu of %lu download(s) "
          "successful.\n",
          successful, download_results.size());

        download_results.clear();

        if (reload && result == UpdateResult::OK)
            result = UpdateResult::RELOAD;

        return result;
    }

    if (! token.empty()) {
        time_t now = nd_time_monotonic();

        if (downloads == 0 &&
          (ttl_last_update == 0 ||
            now > (ttl_last_update + ndGC.ttl_napi_update)))
        {
            ttl_last_update = now;

            for (auto &url : urls) {
                string filename;

                if (url.first == Request::DOWNLOAD_APPLICATIONS)
                    filename = ndGC.path_app_config;
                else if (url.first == Request::DOWNLOAD_APPLICATIONS_DAT)
                    filename = nd_change_ext(ndGC.path_app_config, "dat");
                else if (url.first == Request::DOWNLOAD_CATEGORIES)
                    filename = ndGC.path_cat_config;
                else if (url.first == Request::DOWNLOAD_INTELLIGENCE)
                    filename = ndGC.path_intel_config;
                else if (url.first == Request::DOWNLOAD_INTELLIGENCE_DAT)
                    filename = nd_change_ext(ndGC.path_intel_config, "dat");

                ndNetifyApiDownload *download = new ndNetifyApiDownload(
                  token, url.second, filename);

                auto request = requests.insert(make_pair(url.first, download));

                if (request.second != true) delete download;
                else {
                    try {
                        request.first->second->Create();
                    }
                    catch (exception &e) {
                        nd_printf(
                          "netify-api: Error creating download request: %s\n",
                          e.what());
                        delete download;
                        requests.erase(request.first);
                    }
                }
            }
        }
    }

    return result;
}

void ndNetifyApiManager::Terminate(void) {
    for (auto &request : requests)
        request.second->Terminate();
    for (auto &request : requests) delete request.second;
    requests.clear();
}

string ndNetifyApiManager::GetRequestUrl(ndNetifyApiManager::Request request) {
    unique_lock<mutex> ul(lock);

    string url;
    const auto i = urls.find(request);
    if (i != urls.end()) url = i->second;

    return url;
}

string ndNetifyApiManager::GetToken(void) {
    unique_lock<mutex> ul(lock);
    return token;
}

ndNetifyApiManager::UpdateResult
ndNetifyApiManager::ProcessBootstrapRequest(
  ndNetifyApiBootstrap *bootstrap) {
    jstatus["bootstrap"]["code"] = -1;
    jstatus["bootstrap"]["last_update"] = time(nullptr);

    if (ndCurlParent(bootstrap)->curl_rc != CURLE_OK) {
        jstatus["bootstrap"]["message"] = curl_easy_strerror(
          ndCurlParent(bootstrap)->curl_rc);
        return UpdateResult::ERROR;
    }
    else {
        jstatus["bootstrap"]["message"] = "Unknown result";
    }

    if (ndCurlParent(bootstrap)->http_rc == 0) {
        jstatus["bootstrap"]["code"] = -1;
        jstatus["bootstrap"]["message"] = "Request failure";
        nd_printf(
          "netify-api: Bootstrap request failed.\n");
        return UpdateResult::ERROR;
    }

    if (bootstrap->content.length() == 0) {
        jstatus["bootstrap"]["code"] = -1;
        jstatus["bootstrap"]["message"] = "Empty response";
        nd_printf("netify-api: Empty bootstrap content.\n");
        return UpdateResult::ERROR;
    }

    if (bootstrap->content_type != "application/json") {
        jstatus["bootstrap"]["code"] = -1;
        jstatus["bootstrap"]["message"] =
          "Invalid content type";
        nd_printf(
          "netify-api: Invalid bootstrap content "
          "type.\n");
        return UpdateResult::ERROR;
    }

    try {
        json content = json::parse(bootstrap->content);

        int code = -1;
        string message("Unknown");

        static vector<string> status_codes = {
            "status_"
            "code",
            "resp_code"
        };

        for (auto &key : status_codes) {
            auto ji = content.find(key);
            if (ji != content.end() &&
              (ji->type() == json::value_t::number_integer ||
                ji->type() == json::value_t::number_unsigned))
            {
                code = ji->get<int>();
                break;
            }
        }

        static vector<string> status_messages = {
            "status_message", "resp_message"
        };

        for (auto &key : status_messages) {
            auto ji = content.find(key);
            if (ji != content.end() &&
              ji->type() == json::value_t::string)
            {
                message = ji->get<string>();
                break;
            }
        }

        nd_rtrim(message, '.');
        jstatus["bootstrap"]["code"] = code;
        jstatus["bootstrap"]["message"] = message;

        if (ndCurlParent(bootstrap)->http_rc != 200 || code != 0) {
            nd_printf(
              "netify-api: Bootstrap request failed: HTTP "
              "%ld: %s [%d]\n",
              ndCurlParent(bootstrap)->http_rc, message.c_str(), code);
            return UpdateResult::ERROR;
        }

        auto jdata = content.find("data");
        if (jdata == content.end()) {
            jstatus["bootstrap"]["code"] = -1;
            jstatus["bootstrap"]["message"] =
              "Data not found";
            nd_dprintf(
              "netify-api: Malformed bootstrap content: "
              "%s\n",
              "data not found");
            return UpdateResult::ERROR;
        }

        auto juuid_site = jdata->find("uuid-site");
        if (juuid_site != jdata->end() &&
          juuid_site->type() == json::value_t::string)
        {
            string new_uuid = juuid_site->get<string>();
            if (ndGC.SaveUUID(ndUUID::SITE, new_uuid)) {
                jstatus["bootstrap"]["code"] = 0;
                jstatus["bootstrap"]["message"] =
                  "Site provisioned";
                nd_printf(
                  "netify-api: Site UUID provisioned: %s\n",
                  new_uuid.c_str());
            }
            return UpdateResult::RELOAD_BROADCAST;
        }

        auto jsigs = jdata->find("signatures");
        if (jsigs == jdata->end()) {
            jstatus["bootstrap"]["code"] = -1;
            jstatus["bootstrap"]["message"] =
              "Signatures not found";
            nd_dprintf(
              "netify-api: Malformed bootstrap content: %s\n",
              "signatures not found");
            return UpdateResult::ERROR;
        }

        auto jtoken = jsigs->find("token");
        if (jtoken == jsigs->end() &&
          jtoken->type() != json::value_t::string)
        {
            jstatus["bootstrap"]["code"] = -1;
            jstatus["bootstrap"]["message"] =
              "Authentication token not found";
            nd_dprintf(
              "netify-api: Malformed bootstrap content: %s\n",
              "token not found or invalid type");
            return UpdateResult::ERROR;
        }

        unique_lock<mutex> ul(lock);

        string new_token = jtoken->get<string>();

        if (token.empty() || new_token != token) {
            token = new_token;
            nd_dprintf("netify-api: new API token set.\n");
        }

        auto jendpoint = jsigs->find("applications_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::DOWNLOAD_APPLICATIONS] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("applications_dat_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::DOWNLOAD_APPLICATIONS_DAT] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("categories_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::DOWNLOAD_CATEGORIES] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("intelligence_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::DOWNLOAD_INTELLIGENCE] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("intelligence_dat_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::DOWNLOAD_INTELLIGENCE_DAT] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("feeds_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::FEEDS_FLOWS] = jendpoint->get<string>();
        }

        jendpoint = jsigs->find("feeds_flows_endpoint");
        if (jendpoint != jsigs->end() &&
          jendpoint->type() == json::value_t::string) {
            urls[Request::FEEDS_FLOWS] = jendpoint->get<string>();
        }
    }
    catch (json::exception &e) {
        jstatus["bootstrap"]["code"] = -1;
        jstatus["bootstrap"]["message"] =
          "Exception encountered while assigning signature download URLs";
        nd_printf(
          "netify-api: Failed to decode bootstrap content.\n");
        nd_dprintf("netify-api: JSON exception: %s\n", e.what());
        return UpdateResult::ERROR;
    }

    return UpdateResult::OK;
}

bool ndNetifyApiManager::ProcessDownloadRequest(
  ndNetifyApiDownload *download, Request type) {
    string status_type;

    switch (type) {
    case Request::DOWNLOAD_APPLICATIONS:
        status_type = "applications";
        break;
    case Request::DOWNLOAD_APPLICATIONS_DAT:
        status_type = "applications (dat)";
        break;
    case Request::DOWNLOAD_CATEGORIES:
        status_type = "categories";
        break;
    case Request::DOWNLOAD_INTELLIGENCE:
        status_type = "intelligence";
        break;
    case Request::DOWNLOAD_INTELLIGENCE_DAT:
        status_type = "intelligence (dat)";
        break;
    default:
        nd_dprintf("netify-api: invalid download type: %d", type);
        return false;
    }

    jstatus[status_type]["code"] = ndCurlParent(download)->http_rc;
    jstatus[status_type]["last_update"] = time(nullptr);

    if (ndCurlParent(download)->curl_rc != CURLE_OK) {
        jstatus[status_type]["message"] = curl_easy_strerror(
          ndCurlParent(download)->curl_rc);
        return false;
    }

    switch (ndCurlParent(download)->http_rc) {
    case 200:
        jstatus[status_type]["message"] = "Updated";
        break;
    case 304:
        jstatus[status_type]["message"] =
          "Up-to-date (not modified)";
        return false;
    case 401:
        jstatus[status_type]["message"] =
          "Authorization failure";
        break;
    case 403:
        jstatus[status_type]["message"] =
          "Forbidden request";
        break;
    default:
        jstatus[status_type]["message"] = "Request failure";
        break;
    }

    if (ndCurlParent(download)->http_rc != 200) {
        nd_printf(
          "netify-api: Download request failed: HTTP %ld: "
          "type: %d\n",
          ndCurlParent(download)->http_rc, type);

        if (ndCurlParent(download)->http_rc == 401 ||
            ndCurlParent(download)->http_rc == 403) {
            nd_dprintf(
              "netify-api: cleared token on authorization "
              "failure.\n");
            token.clear();
            ttl_last_update = 0;
        }

        return false;
    }

    if (type == Request::DOWNLOAD_APPLICATIONS) {
        return nd_copy_file(download->content_filename,
          ndGC.path_app_config,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }
    else if (type == Request::DOWNLOAD_APPLICATIONS_DAT) {
        return nd_copy_file(download->content_filename,
          nd_change_ext(ndGC.path_app_config, "dat"),
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }
    else if (type == Request::DOWNLOAD_CATEGORIES) {
        return nd_copy_file(download->content_filename,
          ndGC.path_cat_config,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }
    else if (type == Request::DOWNLOAD_INTELLIGENCE) {
        return nd_copy_file(download->content_filename,
          ndGC.path_intel_config,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }
    else if (type == Request::DOWNLOAD_INTELLIGENCE_DAT) {
        return nd_copy_file(download->content_filename,
          nd_change_ext(ndGC.path_intel_config, "dat"),
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }

    return false;
}
