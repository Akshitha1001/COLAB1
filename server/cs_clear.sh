
source ./.env.common

cd ${REPO_NAME}/${FOLDER_NAME}

#LOAD FUNCTION SCRIPT

source functions.sh

# load_env_variables

del_vm

del_firewall

del_img_gcr

del_cs_files  # delete cloned repo and local image build