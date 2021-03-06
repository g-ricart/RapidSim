#!/bin/bash

PATH=/local/home/gr269908/RapidSim/bin:$PATH; export PATH

export RAPIDSIM_ROOT=/local/home/gr269908/RapidSim/bin/..

mkdir -p plots

echo "RAPIDSIM_ROOT points to ${RAPIDSIM_ROOT}"

RapidSim.exe ${RAPIDSIM_ROOT}/validation/B2Kee 1000 1
RapidSim.exe ${RAPIDSIM_ROOT}/validation/B2Kee 100000
RapidSim.exe ${RAPIDSIM_ROOT}/validation/Bs2Jpsiphi 100000
RapidSim.exe ${RAPIDSIM_ROOT}/validation/Bd2D0rho0 100000
RapidSim.exe ${RAPIDSIM_ROOT}/validation/Bs2D0Kpi 10000
RapidSim.exe ${RAPIDSIM_ROOT}/validation/D02Kpi 100000
RapidSim.exe ${RAPIDSIM_ROOT}/validation/Lb2chicpK 100000

root -b -q -l "${RAPIDSIM_ROOT}/validation/compareHistograms.C(\"B2Kee\")"
root -b -q -l "${RAPIDSIM_ROOT}/validation/compareHistograms.C(\"Bs2Jpsiphi\")"
root -b -q -l "${RAPIDSIM_ROOT}/validation/compareHistograms.C(\"Bd2D0rho0\")"
root -b -q -l "${RAPIDSIM_ROOT}/validation/compareHistograms.C(\"Bs2D0Kpi\")"
root -b -q -l "${RAPIDSIM_ROOT}/validation/compareHistograms.C(\"D02Kpi\")"

