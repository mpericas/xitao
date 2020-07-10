resfile=$1
echo '********************NUMA,RWSS,AVG**********************'
for i in {1..40}
do
	dop=$((6*$i))
	line=$(cat $resfile | sed -n '/numa_start_'$dop'_dop/,/numa_end_'$dop'_dop/p' | grep -I "Assembly Throughput" | grep -Eo "[0-9]+([,.][0-9]+)?" | awk '{ SUM += $1; print $1} END { print SUM/20 }')
	#IFS=' ' read -r -a array <<< "$line"
	array1=($(echo $line | tr ' ' "\n"))
	line=$(cat $resfile | sed -n '/rwss_start_'$dop'_dop/,/rwss_end_'$dop'_dop/p' | grep -I "Assembly Throughput" | grep -Eo "[0-9]+([,.][0-9]+)?" | awk '{ SUM += $1; print $1} END { print SUM/20 }')
	array2=($(echo $line | tr ' ' "\n"))
	echo $dop','"${array1[20]}"','"${array2[20]}"
done

echo '********************NUMA,RWSS,MAX**********************'
for i in {1..40}
do
	dop=$((6*$i))
	line=$(cat $resfile | sed -n '/numa_start_'$dop'_dop/,/numa_end_'$dop'_dop/p' | grep -I "Assembly Throughput" | grep -Eo "[0-9]+([,.][0-9]+)?" | awk '$1 > m || NR == 1 { m = $1 } END { print m }')
	#IFS=' ' read -r -a array <<< "$line"
	array1=($(echo $line | tr ' ' "\n"))
	line=$(cat $resfile | sed -n '/rwss_start_'$dop'_dop/,/rwss_end_'$dop'_dop/p' | grep -I "Assembly Throughput" | grep -Eo "[0-9]+([,.][0-9]+)?" | awk '$1 > m || NR == 1 { m = $1 } END { print m }')
	array2=($(echo $line | tr ' ' "\n"))
	echo $dop','"${array1}"','"${array2}"
done
