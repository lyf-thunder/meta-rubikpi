trap '' INT HUP TERM
if [ ! -f /etc/first-login-done ]; then
    systemctl stop android-tools-adbd.service
    systemctl stop init_display.service
    echo "First time logging in, you must change the root password!"
    while true; do
        passwd root
        if [ $? -eq 0 ]; then
            if [ ! -w /etc ]; then
                echo "Insufficient permissions to write to /etc directory."
                exit 1
            fi
            touch /etc/first-login-done
            echo "Password changed successfully"
            systemctl start android-tools-adbd.service
            systemctl enable android-tools-adbd.service
            systemctl start init_display.service
            break
        else
            echo "Password modification failed, please log in again!"
        fi
    done
fi