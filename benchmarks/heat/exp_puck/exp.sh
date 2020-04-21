XITAO_PTT_PATH=/home/musabdu/xitao/ptt_layouts/puck
INPUTFILE=small.dat

echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-rwss ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-numa ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_2
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-6"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_6
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-2-6"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_2_6
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto ${INPUTFILE}
