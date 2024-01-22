// Netify Agent Risks-to-CSV
// Copyright (C) 2024 eGloo Incorporated
// <http://www.egloo.ca>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include "nd-ndpi.hpp"
#include "nd-risks.hpp"

int main(int argc, char* argv[]) {
    int rc = 0;

    ndpi_global_init();

    cout << "Netify ID,nDPI ID,Risk,Severity,Score,Client "
            "Score,Server Score"
         << endl;

    for (uint16_t id = 1; id < NDPI_MAX_RISK; id++) {
        auto it = nd_ndpi_risks.find(id);
        if (it == nd_ndpi_risks.end()) continue;

        ndpi_risk_enum rid = (ndpi_risk_enum)id;

        ndpi_risk_info const* const risk_info =
          ndpi_risk2severity(rid);
        if (risk_info == nullptr) continue;

        const char* risk = ndpi_risk2str(rid);
        const char* severity = ndpi_severity2str(risk_info->severity);

        uint16_t client_score = 0, server_score = 0;
        uint16_t score = ndpi_risk2score(rid, &client_score,
          &server_score);

        cout << id << "," << rid << "," << risk << ","
             << severity << "," << score << ","
             << client_score << "," << server_score << endl;
    }

    return 0;
}
