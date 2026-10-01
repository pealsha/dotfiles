# Shared versions and paths for the Ubuntu / WSL C++ environment.
CP_CXX=g++-13
CP_STD=gnu++23
CP_TEMPLATE_DIR="$HOME/.config/atcoder-cli-nodejs/cpp"
CP_ACL_DIR="$HOME/lib/ac-library-master"
CP_ACC_VERSION=2.2.0
CP_OJ_VERSION=11.5.1
CP_OJ_API_VERSION=10.10.1
CP_ACL_REV=79b2f930f50884c05b87d51699e98ca366233cbf

# Machine-specific overrides are intentionally unmanaged.
if [ -f "$HOME/.config/competitive-programming/local.sh" ]; then
    . "$HOME/.config/competitive-programming/local.sh"
fi
