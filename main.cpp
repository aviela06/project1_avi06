#include <iostream>
#include <stdlib.h>
#include "capacitor.h"

int main()
{
    double dt = 1e-10;       // time step
    double final_time = 5e-6;
    int nsteps = 50000;
    double R = 1000.0;       // resistance
    double C = 100e-12;      // capacitance
    double I0 = 1e-2;        // constant current supply
    double V0 = 10.0;        // constant voltage supply

    Capacitor cc; // constant current circuit
    Capacitor cv; // constant voltage circuit

    // allocating memory for the arrays
    cc.time = (double *) malloc(sizeof(double) * (nsteps + 1));
    cc.voltage = (double *) malloc(sizeof(double) * (nsteps + 1));
    cc.current = (double *) malloc(sizeof(double) * (nsteps + 1));

    cv.time = (double *) malloc(sizeof(double) * (nsteps + 1));
    cv.voltage = (double *) malloc(sizeof(double) * (nsteps + 1));
    cv.current = (double *) malloc(sizeof(double) * (nsteps + 1));

    cc.C = C; // adding capacitance to structure
    cv.C = C; // adding capacitance to structure

    // initial conditions at step 0
    cc.time[0] = 0.0;
    cc.voltage[0] = 0.0;
    cc.current[0] = I0;

    cv.time[0] = 0.0;
    cv.current[0] = V0 / R;
    cv.voltage[0] = V0 - cv.current[0] * R;

    for (int i = 1; i <= nsteps; i++) // loop over each time step
    {
        cc.time[i] = i * dt; // time at this step for constant current
        cv.time[i] = i * dt; // time at this step for constant voltage

        // constant current: current stays fixed, voltage increases
        cc.current[i] = I0;
        cc.voltage[i] = cc.voltage[i - 1] + cc.current[i - 1] * dt / cc.C;

        // constant voltage: current decays, voltage from KVL
        cv.current[i] = cv.current[i - 1] - cv.current[i - 1] / (R * cv.C) * dt;
        cv.voltage[i] = V0 - cv.current[i] * R;

        if (i % 200 == 0) // print every 200 steps
        {
            std::cout << "time = " << cc.time[i]
                      << " | CC voltage = " << cc.voltage[i]
                      << " CC current = " << cc.current[i]
                      << " | CV voltage = " << cv.voltage[i]
                      << " CV current = " << cv.current[i] << "\n";
        }
    }

    // free all memory
    free(cc.time);
    free(cc.voltage);
    free(cc.current);
    free(cv.time);
    free(cv.voltage);
    free(cv.current);

    return 0;
}
