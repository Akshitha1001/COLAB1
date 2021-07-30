
# GET ENV VARIABLES
load_env_variables() {

    set -o allexport
    source .env.common
    source .env.credentials
    set +o allexport
}

# LOGIN TO GCLOUD
auth_gcloud(){

  R=`gcloud auth list --filter-account=${GCLOUD_EMAIL}`
  if [[ -z $R ]]
  then
    gcloud auth login
  fi
}

# CLONE REPO IN CLOUD SHELL
clone_repo_cs(){

    REPO_URL="github.com/${REPO_OWNER}/${REPO_NAME}.git"
    gcloud cloud-shell ssh --authorize-session --command="git clone http://${GITHUB_USERNAME}:${GITHUB_PASSWORD}@${REPO_URL}"
}

upload_env_file(){
    
    gcloud cloud-shell scp localhost:./.env.common cloudshell:.env.common
    gcloud cloud-shell ssh --authorize-session --command="source ./.env.common"
}

# READ INPUT FROM USER
read_input(){

    echo "1. Deploy VM"
    echo "2. Delete VM"
    echo "Your Choice"

    read -p "Your Choice : " CHOICE
    return $CHOICE
}

# EXECUTE DEPLOY SCRIPT ON CLOUD SHELL
exe_deploy_cs(){

    # MAKE sh FILE EXECUTABLE
    gcloud cloud-shell ssh --authorize-session --command="chmod +x ${REPO_NAME}/server/cs_deploy.sh"

    # RUN COMANDS ON CLOUD SHELL
    gcloud cloud-shell ssh --authorize-session --command="${REPO_NAME}/server/cs_deploy.sh"
}

# EXECUTE CLEAR SCRIPT ON CLOUD SHELL
exe_clean_cs(){

    # MAKE sh FILE EXECUTABLE
    gcloud cloud-shell ssh --authorize-session --command="chmod +x ${REPO_NAME}/server/cs_clear.sh"

    # RUN COMANDS ON CLOUD SHELL
    gcloud cloud-shell ssh --authorize-session --command="${REPO_NAME}/server/cs_clear.sh"
}

# CD TO SERVER
cd_server(){

    cd ${REPO_NAME}/${FOLDER_NAME}
}

#CREATE PROJECT
create_project(){

    gcloud projects create ${GCP_PROJECT_NAME}
}


# CREATE DOCKER IMAGE IN CONTAINER REPOSITORY
push_to_gcr(){

    IMAGE=${HOSTNAME}/${GCP_PROJECT_NAME}/${REPO_NAME}

    docker build -t ${IMAGE} .

    docker push ${IMAGE}
}

# CREATE FIREWALL
create_firewall(){

    gcloud compute --project=${GCP_PROJECT_NAME} \
                firewall-rules create ${GCP_PROJECT_NAME}-firewall \
                --direction=INGRESS --priority=999 \
                --network=default --action=ALLOW \
                --rules=tcp:5000,tcp:8080 \
                --source-ranges=0.0.0.0/0
}

# CREATE VM
create_vm(){

    gcloud beta compute --project=${GCP_PROJECT_NAME} instances \
        create-with-container ${GCP_PROJECT_NAME}-vm --zone=${ZONE} \
        --machine-type=e2-micro --subnet=default --network-tier=PREMIUM \
        --metadata=google-logging-enabled=true --maintenance-policy=MIGRATE \
        --service-account=262821520792-compute@developer.gserviceaccount.com \
        --scopes=https://www.googleapis.com/auth/devstorage.read_only,https://www.googleapis.com/auth/logging.write,https://www.googleapis.com/auth/monitoring.write,https://www.googleapis.com/auth/servicecontrol,https://www.googleapis.com/auth/service.management.readonly,https://www.googleapis.com/auth/trace.append \
        --image=cos-stable-89-16108-470-11 --image-project=cos-cloud \
        --boot-disk-size=10GB --boot-disk-type=pd-balanced \
        --boot-disk-device-name=hello-flask-vm --no-shielded-secure-boot \
        --shielded-vtpm --shielded-integrity-monitoring --container-image=${IMAGE} \
        --container-restart-policy=always --labels=container-vm=cos-stable-89-16108-470-11
}

# DELETE THE LOCAL IMAGE & CLONED REPO
del_cs_files(){

    docker rmi ${IMAGE}
    cd ../..
    rm -rf ${REPO_NAME}
}

# DELETE VM
del_vm(){

    gcloud compute instances delete ${GCP_PROJECT_NAME}-vm
}

del_firewall(){

    gcloud compute firewall-rules delete ${GCP_PROJECT_NAME}-firewall
}

# DELETE IMAGE FROM CONTAINER REGISTRY
del_img_gcr(){

    gcloud container images delete ${IMAGE}
}