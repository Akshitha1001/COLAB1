
source ./.env.common

cd ${REPO_NAME}/${FOLDER_NAME}

#LOAD FUNCTION SCRIPT
source functions.sh

push_to_gcr

create_firewall

create_vm

#del_cs_files   # delete cloned repo and local image build


