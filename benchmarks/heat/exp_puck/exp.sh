
echo "****************************************************************************************************"
echo "RWSS"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-rwss small.dat
echo "****************************************************************************************************"
echo "NUMA-Aware"
echo "****************************************************************************************************"
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-numa small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-No-Mold"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=/home/musabdu/xitao/ptt_layout_puck_1
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-2"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=/home/musabdu/xitao/ptt_layout_puck_2
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-6"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=/home/musabdu/xitao/ptt_layout_puck_6
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-numa-auto small.dat
echo "****************************************************************************************************"
echo "NUMA-Auto-Mold-2-6"
echo "****************************************************************************************************"
export XITAO_LAYOUT_PATH=/home/musabdu/xitao/ptt_layout_puck_2_6
TAO_NOPLO=T1 AWIDTH=6 EXDECOMP=8 EYDECOMP=6 IXDECOMP=6 IYDECOMP=1 numactl -i all ./heat-tao-numa-auto small.dat
