#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
  echo "Usage: ./grade.sh <filename>"
  exit 0
fi

# Delete temporary files
rm -f ./ref/*.out

# Compile the reference program
FILES=""

for i in ./ref/*.c; do
  FILES="${FILES} $i"
done

gcc $FILES -o $1

# Generate reference output files
TOTAL=0
for i in ./ref/*.in; do
  ./$1 < $i > ./"${i%%.in}.out"
  TOTAL=$((TOTAL+1))
done

# Now mark submissions
echo -e "Test date and time: $(date +%A,\ %d\ %B\ %Y,\ %T)\n" > results.out
NUM_FILES=0
for i in ./subs/*/; do
  FILES=""

  for j in $i*.c; do
    FILES="${FILES} $j"
  done

  MATRIC_NUMBER=${i%/}
  MATRIC_NUMBER=${MATRIC_NUMBER##*/}
  gcc $FILES &> /dev/null

  if [[ $? -ne 0 ]]; then
    echo "Directory $MATRIC_NUMBER has a compile error." >> results.out
    continue
  else
    mv a.out $i
  fi

  SCORE=0
  for j in ./ref/*.in; do
    OUTPUT_FILE=$j
    OUTPUT_FILE=${OUTPUT_FILE##*/}
    OUTPUT_FILE="${OUTPUT_FILE%%.in}.out"
    $i/./a.out < $j > $i"$OUTPUT_FILE"
    diff -q $i"$OUTPUT_FILE" ./ref/"$OUTPUT_FILE" > /dev/null 
    if [[ $? -eq 0 ]]; then
      SCORE=$((SCORE+1))
    fi
  done

  echo "Directory $MATRIC_NUMBER score $SCORE / $TOTAL" >> results.out
  NUM_FILES=$((NUM_FILES+1))
done

echo -e "\nProcessed $NUM_FILES files." >> results.out

# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#

# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file
    # Generate output from C code using *.in files in ref
    # Compare with reference output files  and award 1 mark if they are identical
# print score for student
# print total files marked.
    
