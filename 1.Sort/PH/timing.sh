rm Task1
rm Task1OMP
rm time

export OMP_NUM_THREADS=8 && g++ -fopenmp Task1.c -o Task1
export OMP_NUM_THREADS=8 && g++ -fopenmp Task1OMP.c -o Task1OMP

for CURRTIME in {0..1000}
do
	(echo "Single" && time ./Task1 $CURRTIME && echo "" && echo "OMP"&& time ./Task1OMP $CURRTIME && echo "") &>> time
done

rm Task1
rm Task1OMP