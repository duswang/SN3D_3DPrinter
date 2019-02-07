clear


sudo df -l
sudo fdisk -l /dev/mmcblk0

sudo dd if=/dev/mmcblk0 of=/mnt/volume/SN3D_RBP.img bs=512 count=8290303 status=progress
