#!/bin/sh

echo mpiexec -hostfile $(dirname $0)/hostfile $@
mpiexec -hostfile $(dirname $0)/hostfile $@
