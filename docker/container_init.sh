#!/bin/bash -e
# One time setup for the target user to be run within the container as the "entrypoint"

mount_point=$REPO_ROOT

target_username="${TARGET_USER}"
target_uid="$(stat --printf=%u ${mount_point})"
target_gid="$(stat --printf=%g ${mount_point})"

groupadd -g ${target_gid} ${target_username}
useradd -No \
        -s /bin/bash \
        -d ${mount_point} \
        -u ${target_uid} \
        -g ${target_gid} \
        -G sudo,video \
        ${target_username}

# ln -s $REPO_ROOT/docker/.bashrc $REPO_ROOT/.bashrc

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
