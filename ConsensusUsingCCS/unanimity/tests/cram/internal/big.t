This is a larger scale test run on a nightly basis.  It runs a good
chunk of a movie through CCS and pushes the results through ccscheck.
For now we just watch that the results don't change; in the future we
might like to replace that with some bulk checks on alignment identity,
for example.

Run CCS:

  $ DATADIR=/pbi/dept/consensus/testdata/unanimity-nightly

  $ mkdir results
  $ ${__PBTEST_CCS_EXE} --zmws 1-187412 $DATADIR/ds.subreadset.xml out.bam

Run ccscheck, check output:

  $ /pbi/dept/consensus/ccscheck/bin/ccscheck out.bam stats /pbi/dept/consensus/references/lambdaNEB.fasta
  $ sort -t, -n -k1,1 -k2,2 stats/zmws.csv > zmws.sorted.csv
  $ diff -NrU1 zmws.sorted.csv /pbi/dept/consensus/testdata/unanimity-nightly/zmws.sorted.to187412.csv

TODO: copy back to the "lastrun" dir
