#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
  echo "Usage: ./grade.sh <filename>"
  exit 2
fi

# Delete temporary files
rm -f ./ref/*.out

# Compile the reference program
gcc ./ref/*.c -o $1

# Generate reference output files
for i in ./ref/*.in; do
  ./$1 < $i > ./"${i%%.in}.out"
done

TOTAL=$(ls -1 ./ref/ | grep .in | wc -l)

# Now mark submissions
echo -e "Test date and time: $(date +%A,\ %d\ %B\ %Y,\ %T)\n" > results.out

NUM_FILES=0

for i in ./subs/*/; do
  MATRIC_NUMBER=$(basename ./subs/$i)
  gcc $i*.c &> /dev/null
  
  if [[ $? -ne 0 ]]; then
    echo "Directory $MATRIC_NUMBER has a compile error." >> results.out
    continue
  else
    mv a.out $i
  fi

  SCORE=0
  for j in ./ref/*.in; do
    TEST_CASE=$(basename -- $j .in)
    OUTPUT_FILE="${TEST_CASE}.out"
    $i/./a.out < $j > $i"$OUTPUT_FILE"
    diff -q $i"$OUTPUT_FILE" ./ref/"$OUTPUT_FILE" > /dev/null 
    if [[ $? -eq 0 ]]; then
      let "SCORE++"
    fi
  done

  echo "Directory $MATRIC_NUMBER score $SCORE / $TOTAL" >> results.out
  let "NUM_FILES++"
done

echo -e "\nProcessed $NUM_FILES files." >> results.out

cat results.out

# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#

# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file
    # Generate output from C code using *.in files in ref
    # Compare with reference output files  and award 1 mark if they are identical
# print score for student
# print total files marked.
    
