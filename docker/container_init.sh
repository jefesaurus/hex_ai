#!/bin/bash -e
# One time setup for the target user to be run within the container as the "entrypoint"

home_dir=$REPO_ROOT/docker

target_username="${TARGET_USER}"
target_uid="$(stat --printf=%u ${home_dir})"
target_gid="$(stat --printf=%g ${home_dir})"

groupadd -g ${target_gid} ${target_username}
useradd -No \
        -s /bin/bash \
        -d ${home_dir} \
        -u ${target_uid} \
        -g ${target_gid} \
        -G sudo,video \
        ${target_username}

cat > /etc/sudoers <<-EOF
	Defaults    !secure_path
	Defaults    env_reset

	Defaults    env_keep += "TARGET_USER"
	Defaults    env_keep += "DISPLAY"

	root        ALL=(ALL:ALL) ALL
	%sudo       ALL=(ALL:ALL) NOPASSWD:ALL
EOF
visudo -cq
exec sudo -Eiu ${target_username} "${@}"
