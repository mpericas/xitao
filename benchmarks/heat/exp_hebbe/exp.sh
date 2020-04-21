XITAO_PTT_PATH=/home/musabdu/xitao/ptt_layouts/hebbe
INPUTFILE=vsmall.dat

echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-rwss ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_2
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5-10"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5_10
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}


XITAO_PTT_PATH=/home/musabdu/xitao/ptt_layouts/hebbe
INPUTFILE=small.dat

echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-rwss ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_2
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5-10"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5_10
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}


XITAO_PTT_PATH=/home/musabdu/xitao/ptt_layouts/hebbe
INPUTFILE=big.dat

echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-rwss ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_2
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5-10"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5_10
TAO_NOPLOT=1 AWIDTH=2 EXDECOMP=20 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
