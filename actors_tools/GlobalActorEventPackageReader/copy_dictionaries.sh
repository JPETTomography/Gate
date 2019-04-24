CURRENT_DIRECTORY=$1
FILES_TO_COPY_LIST=files_include.dat
FILES_LS_LIST=files_ls_list.dat

ls -l $1/../../source/digits_hits/include/GateGlobalActorDictionar* >> ${FILES_LS_LIST}

ls -l $1/../../source/digits_hits/src/GateGlobalActorDictionar* >> ${FILES_LS_LIST}
awk '{print $9}' ${FILES_LS_LIST} >> ${FILES_TO_COPY_LIST}

rm ${FILES_LS_LIST}

while IFS='' read -r line || [[ -n "$line" ]]; do
    cp ${line} ${CURRENT_DIRECTORY}/Dictionaries
done < "${FILES_TO_COPY_LIST}"

rm ${FILES_TO_COPY_LIST}
