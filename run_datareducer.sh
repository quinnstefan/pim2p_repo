echo "What's the input file, input tree, and outputfile?"

INPUTFILE=../flattree_pim2p_090215.root
OUTPUTFILE=test_090215.root

root -b -q 'datareducer.C("'${INPUTFILE}'","'${OUTPUTFILE}'")'
