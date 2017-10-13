if [[ -z "$REPO_ROOT" ]]
then
    REPO_SETUP_SH=${BASH_SOURCE[0]}
    REPO_ROOT="$(cd "$(dirname "$REPO_SETUP_SH")" && pwd -P)"
fi
export REPO_ROOT
export PATH=$REPO_ROOT/tools/bin:$PATH
export PYTHONPATH=$REPO_ROOT:$PYTHONPATH
