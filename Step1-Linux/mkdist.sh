#!/bin/bash
# This shell script creates a bootable Qemu disk, in a file.
# First, it creates a disk image (raw) and sets up GRUB.
# Then, it tweaks with the MBR to create a partition
# and makes an ext2 file system on it.
# Finally, it mounts that partition and copies your mini
# distribution.
#
# This script will show you how to do the following:
#  - use dd
#  - use fdisk & parted
#  - control a program through a script (redirection)
#  - use losetup to mount a file as a disk partition
#    or use mount with -o loop option.

GRUB_DIR=./grub-0.97
DIST_DIR=./MiniDist
MOUNT_DIR=/mnt/hello
DISK=disk.img

# Color definitions for colored output via echo.
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
NOCOLOR='\033[0m'

# Function to check if the last command was successful,
# it is called with the line number at the check was done.
function check() {
		if [ $? -ne 0 ] ; then
				echo -e -n "${RED}"
				echo "PANIC: line $1"
				# attempt to clean the loop device if it was created.
				if [ ! -z $DEVLOOP ] ; then
						echo "Cleaning loop device $DEVLOOP"
						mount | grep $DEVLOOP >> /dev/null 2>&1
						if [ $? -eq 0 ] ; then
								sudo umount $DEVLOOP
						fi
						sudo losetup -d $DEVLOOP
				fi		
				echo -e -n "${NOCOLOR}"
				exit 1
		fi  
}


echo "======================================================================"
echo "Building hello..."
echo "======================================================================"

# First of all, do compile the hello program that will be used
# as the init process when booting just a bare kernel, without 
# any distribution:

(cd hello ; make -s clean ; make -s )
if [ $? -ne 0 ] ; then
    echo "Failed compiling hello..."
    echo "You probably need to install the multilib support."
    echo "   sudo dpkg --add-architecture i386"
    echo "   sudo apt-get install gcc-multilib"
    exit -1
fi

cp hello/hello $DIST_DIR/
check $LINENO
mkdir -p $DIST_DIR/boot
check $LINENO
cp hello/initrd.hello $DIST_DIR/boot/initrd.hello
check $LINENO

echo "======================================================================"
echo "Creating the disk image..."
echo "======================================================================"

# Create the raw disk image...
dd if=/dev/zero of=$DISK bs=512 count=256000 seek=256
check $LINENO

echo "======================================================================"
echo "Installing GRUB MBR, called stage1 "
echo "======================================================================"

# Copy the GRUB stage1 (Master Boot Record)
# Notice the "notrunc" option... 

dd conv=notrunc if=$GRUB_DIR/stage1 of=$DISK bs=512 count=1
check $LINENO

#----------------------------------------------------------
# First experiment, we could put GRUB stage2 on the disk
# it would be enough to boot GRUB, but then, it is not enough
# to do anything else. But still, you may want to try it,
# just uncomment the following three lines:
#----------------------------------------------------------
# dd conv=notrunc if=$GRUB_DIR/stage2 of=$DISK bs=512 seek=1
# qemu-system-i386 -m 256 -hda $DISK -serial stdio 
# exit 0

echo "======================================================================"
echo "Partitioning the disk image..."
echo "======================================================================"

which parted
if [ $? -ne 0 ] ; then
  echo "Missing the disk utility: parted"
  echo "Please install it:"
  echo "   sudo apt-get install parted"
  exit -1
fi

# Create an msdos label, always the first step in creating a disk.
parted -s $DISK mklabel msdos

# Create a primary parttion, starting at sector 256, taking all the
# remaining sectors.
parted -s $DISK mkpart primary ext2 256s 256000s
check $LINENO

echo "======================================================================"
echo "Loop mount the disk image..."
echo "======================================================================"

# Grab available loop device
DEVLOOP=`losetup -f`
check $LINENO

sudo losetup -o131072 $DEVLOOP $DISK
check $LINENO

# the above command could have been written 
#     $ sudo losetup -o$((256*512)) /dev/loop1 $DISK
# since 256*512=131072

echo "======================================================================"
echo "Creating the file system..."
echo "======================================================================"

# create a file system in our partition, using the ext2 format
sudo mkfs -t ext2 $DEVLOOP
check $LINENO

# mounting that file system
sudo mkdir -p $MOUNT_DIR
check $LINENO
sudo mount $DEVLOOP $MOUNT_DIR
check $LINENO

echo "======================================================================"
echo "Copying the mini-dist contents..."
echo "======================================================================"

# Now copy the mini distribution onto the partition we just mounted.
# Pay attention at how we use tar and not a copy to preserve
# symbolic links, ownership, and access rights.

( cd $DIST_DIR ; sudo tar -cf - . | (cd $MOUNT_DIR ; sudo tar -xf - ))
check $LINENO

# The previous command could have been written:
#   ( cd $DIST_DIR ; sudo cp -ar . $MOUNT_DIR )

echo "======================================================================"
echo "Syncing the mounted file systems, "
echo "before unmounting and destructing our loop device..."

# Sync the file system buffers with the underlying devices:
sync

# Unmount our partition and free the loop device we created
sudo umount $DEVLOOP
check $LINENO
sudo losetup -d $DEVLOOP
check $LINENO

echo "======================================================================"
echo "Installing GRUB..."
echo "======================================================================"

# The installation of GRUB can be done manually or automatically.
# Try the manual install once, to see it at least once in your life ;-)
# Then switch to the automated installation. Recognize in the automated
# scripting the very same steps you took when installing manually.

# GRUB_INSTALL="MANUAL"
GRUB_INSTALL="AUTO"

if [ $GRUB_INSTALL = "MANUAL" ] ; then

  echo "======================================================================"
  echo " GRUB - MANUAL INSTALL"
  echo "======================================================================"

  # Qemu BIOS would read the MBR that has the GRUB stage1
  # and the stage1 will load GRUB stage2.
  # The stage2 will give a GRUB shell because GRUB has
  # not properly self-installed on that device.
  #
  # In the GRUB shell, issue the following commands:
  #   grub> root (hd0,0)
  #   grub> setup (hd0)
  #   grub> halt
  
  qemu-system-i386 -m 256 -serial stdio -drive format=raw,file=$DISK 

else

  echo "======================================================================"
  echo " GRUB - AUTOMATED INSTALL"
  echo "======================================================================"
  
  # Fortunately, we can automate the installation of GRUB,
  # using GRUB itself, directly from Linux.
  # We need to create a device map that maps GRUB hd0 to our disk.
  DEVICE_MAP=/tmp/device.map
  rm -f $DEVICE_MAP
  cat >> $DEVICE_MAP <<EOF
(hd0) $DISK          
EOF
  check $LINENO
        
  # Now, we need to start the GRUB shell, using the device map
  # we just generated. By re-directing the input, we can automate
  # the install, hoping there will be no errors.

  $GRUB_DIR/grub  --device-map=$DEVICE_MAP <<EOF
root (hd0,0)
setup (hd0)
quit
EOF
  check $LINENO

  # We can bo
  rm -f $DEVICE_MAP
fi

echo "======================================================================"
echo "Booting QEMU on our disk image..."
echo "you should see the GRUB menu "
echo "that is part of your Mini-distribution"
echo "under MiniDist/boot/grub/menu.lst"
echo ""
qemu-system-i386 -m 256 -hda $DISK -serial mon:stdio 

exit 0


