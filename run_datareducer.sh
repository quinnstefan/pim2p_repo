echo "What's the input file, input tree, and outputfile?"

INPUTFILE=flattree_pim2p_090215.root
INPUTTREE=gd_pimprotprotinc__B4_F4_T1_S4
OUTPUTFILE=test_090215.root

root -b -q 'datareducer.C("'${INPUTFILE}'","'${INPUTTREE}'","'${OUTPUTFILE}'")'
