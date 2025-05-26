
if [ ! -f /etc/first-login-done ]; then

    systemctl stop init_display.service
    echo "First time logging in, you must change the root password!"
    passwd root

    if [ $? -eq 0 ]; then
        if [ ! -w /etc ]; then
            echo "Insufficient permissions to write to /etc directory."
            exit 1
        fi
        touch /etc/first-login-done
        echo "Password changed successfully"
        systemctl start init_display.service
    else
        echo "Password modification failed, please log in again!"
        exit 1
    fi
fi
