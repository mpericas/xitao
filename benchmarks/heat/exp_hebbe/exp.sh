XITAO_PTT_PATH=/home/musabdu/xitao/ptt_layouts/hebbe

echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-rwss small.dat
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_2
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-1-5-10"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=${XITAO_PTT_PATH}/ptt_layout_1_5_10
TAO_NOPLO=T1 AWIDTH=2 EXDECOMP=10 EYDECOMP=2 IXDECOMP=2 IYDECOMP=1 numactl -i all ../heat-tao-numa-auto small.dat
