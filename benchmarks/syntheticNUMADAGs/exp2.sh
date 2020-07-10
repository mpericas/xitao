rep=20
numtasks=$((48*1000))
for i in {1..40} 
do

	a=$((6*$i))
	depth=$(($numtasks/$a))
	echo 'rwss_start_'$a'_dop' 
	echo '********************************************'
	echo 'RWSS'
	echo '********************************************'
        for j in $(seq $rep); do
		./numabench-rwss 512 1 $a $depth 1 2
		echo '********************************************'
	done 
	echo 'rwss_end_'$a'_dop' 
	echo 'numa_start_'$a'_dop' 
	echo '********************************************'
	echo 'NUMA'
	echo '********************************************'
	for j in $(seq $rep); do
		./numabench-numa 512 1 $a $depth 1 2
		echo '********************************************'
	done 
	echo 'numa_end_'$a'_dop' 

done

