/*
 * author: Wenbin Song
 * date: 7/21/2015
 * eMail: <songwenbin247@163.com>
 */

#ifndef __CONFIG_TINY4412_H
#define __CONFIG_TINY4412_H
#include <configs/exynos4-common.h>
/*
 * High Level Configuration Options
 * (easy to change)
 */

#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"TINY4412 # "

#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

#define CONFIG_MACH_TYPE  4289

#define CONFIG_NR_DRAM_BANKS 2
#define CONFIG_SYS_SDRAM_BASE 0x40000000
#define SDRAM_BANK_SIZE      (512 << 20)
#define PHYS_SDRAM_1    CONFIG_SYS_SDRAM_BASE

/*Reserve the last 1MB for the secure firmware*/
#define CONFIG_SYS_MEM_TOP_HIDE (1UL << 20UL)
#define CONFIG_TZSW_RESERVED_DRAM_SIZE CONFIG_SYS_MEM_TOP_HIDE

/*memtest works on*/
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5e00000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)
#define CONFIG_SYS_TEXT_BASE		0x43e00000

#include <linux/sizes.h>

/* select serial console configuration */
#define CONFIG_SERIAL1
#define CONFIG_BAUDRATE			115200

/* Console configuration */
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_SYS_CONSOLE_IS_IN_ENV


#define CONFIG_CMD_BOOTZ
#define CONFIG_FIT
#define CONFIG_FIT_VERBOSE
#define CONFIG_BOOTARGS			"Please use defined boot"
#define CONFIG_BOOTCOMMAND		"run autoboot"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC1,115200n8\0"

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR \
					- GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MONITOR_BASE	0x00000000

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV
#define CONFIG_ENV_SIZE			4096
#define CONFIG_ENV_OFFSET		(SZ_1K * 1280) /* 1.25 MiB offset */
#define CONFIG_ENV_OVERWRITE

/* Partitions name */
#define PARTS_BOOT		"boot"
#define PARTS_ROOT		"platform"

#define CONFIG_DFU_ALT \
	"uImage fat 0 1;" \
	"zImage fat 0 1;" \
	"Image.itb fat 0 1;" \
	"uInitrd fat 0 1;" \
	"exynos4412-odroidu3.dtb fat 0 1;" \
	"exynos4412-odroidx2.dtb fat 0 1;" \
	""PARTS_BOOT" part 0 1;" \
	""PARTS_ROOT" part 0 2\0" \

#define CONFIG_SET_DFU_ALT_INFO
#define CONFIG_SET_DFU_ALT_BUF_LEN	(SZ_1K)

#define CONFIG_DFU_ALT_BOOT_EMMC \
	"u-boot raw 0x3e 0x800 mmcpart 1;" \
	"bl1 raw 0x0 0x1e mmcpart 1;" \
	"bl2 raw 0x1e 0x1d mmcpart 1;" \
	"tzsw raw 0x83e 0x138 mmcpart 1\0"

#define CONFIG_DFU_ALT_BOOT_SD \
	"u-boot raw 0x3f 0x800;" \
	"bl1 raw 0x1 0x1e;" \
	"bl2 raw 0x1f 0x1d;" \
	"tzsw raw 0x83f 0x138\0"

/*
 * Bootable media layout:
 * dev:    SD   eMMC(part boot)
 * BL1      1    0
 * BL2     31   30
 * UBOOT   63   62
 * TZSW  2111 2110
 * ENV   2560 2560(part user)
 *
 * MBR Primary partiions:
 * Num Name   Size  Offset
 * 1.  BOOT:  100MiB 2MiB
 * 2.  ROOT:  -
*/
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadkernel=fatload mmc ${mmcbootdev}:${mmcbootpart} ${kerneladdr} " \
		"${kernelname}\0" \
	"loadinitrd=fatload mmc ${mmcbootdev}:${mmcbootpart} ${initrdaddr} " \
		"${initrdname}\0" \
	"loaddtb=fatload mmc ${mmcbootdev}:${mmcbootpart} ${fdtaddr} " \
		"${fdtfile}\0" \
	"check_ramdisk=" \
		"if run loadinitrd; then " \
			"setenv initrd_addr ${initrdaddr};" \
		"else " \
			"setenv initrd_addr -;" \
		"fi;\0" \
	"check_dtb=" \
		"if run loaddtb; then " \
			"setenv fdt_addr ${fdtaddr};" \
		"else " \
			"setenv fdt_addr;" \
		"fi;\0" \
	"kernel_args=" \
		"setenv bootargs root=/dev/mmcblk${mmcrootdev}p${mmcrootpart}" \
		" rootwait ${console} ${opts}\0" \
	"boot_fit=" \
		"setenv kerneladdr 0x42000000;" \
		"setenv kernelname Image.itb;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootm ${kerneladdr}#${boardname}\0" \
	"boot_uimg=" \
		"setenv kerneladdr 0x40007FC0;" \
		"setenv kernelname uImage;" \
		"run check_dtb;" \
		"run check_ramdisk;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootm ${kerneladdr} ${initrd_addr} ${fdt_addr};\0" \
	"boot_zimg=" \
		"setenv kerneladdr 0x40007FC0;" \
		"setenv kernelname zImage;" \
		"run check_dtb;" \
		"run check_ramdisk;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootz ${kerneladdr} ${initrd_addr} ${fdt_addr};\0" \
	"autoboot=" \
		"if test -e mmc 0 Image.itb; then; " \
			"run boot_fit;" \
		"elif test -e mmc 0 zImage; then; " \
			"run boot_zimg;" \
		"elif test -e mmc 0 uImage; then; " \
			"run boot_uimg;" \
		"fi;\0" \
	"console=" CONFIG_DEFAULT_CONSOLE \
	"mmcbootdev=0\0" \
	"mmcbootpart=1\0" \
	"mmcrootdev=0\0" \
	"mmcrootpart=2\0" \
	"bootdelay=0\0" \
	"dfu_alt_system="CONFIG_DFU_ALT \
	"dfu_alt_info=Please reset the board\0" \
	"consoleon=set console console=ttySAC1,115200n8; save; reset\0" \
	"consoleoff=set console console=ram; save; reset\0" \
	"initrdname=uInitrd\0" \
	"initrdaddr=42000000\0" \
	"fdtaddr=40800000\0"

/* I2C */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_S3C24X0
#define CONFIG_SYS_I2C_S3C24X0_SPEED	100000
#define CONFIG_SYS_I2C_S3C24X0_SLAVE	0

/* GPT */
#define CONFIG_RANDOM_UUID

/* Security subsystem - enable hw_rand() */
#define CONFIG_EXYNOS_ACE_SHA
#define CONFIG_LIB_HW_RAND

#define CONFIG_CMD_GPIO

/* USB */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_EXYNOS
#define CONFIG_USB_STORAGE

#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	3
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_USB_HOST_ETHER

#define CONFIG_MISC_COMMON
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_BOARD_TYPES
#define CONFIG_MISC_INIT_R

#undef CONFIG_REVISION_TAG

#endif	/* __CONFIG_H */


















//
//#define CONFIG_TRUSTZONE
//#define CONFIG_TRUSTZONE_RESERVED_DRAM	0x100000
//
//#define CONFIG_SECURE_BL1_ONLY
////#define CONFIG_SECURE_BOOT
//#ifdef CONFIG_SECURE_BOOT
//#define CONFIG_S5PC210S
//#define CONFIG_SECURE_ROOTFS
//#define CONFIG_SECURE_KERNEL_BASE	0x40008000
//#define CONFIG_SECURE_KERNEL_SIZE	0x300000
//#define CONFIG_SECURE_ROOTFS_BASE	0x41000000
//#define CONFIG_SECURE_ROOTFS_SIZE	0x100000
//#endif
//
////#define CONFIG_UPDATE_SOLUTION	1
//
////#include <asm/arch/cpu.h>		/* get chip and board defs */
//
///* APLL : 800MHz */
////#define CONFIG_CLK_ARM_800_APLL_800
///* APLL : 1GHz */
////#define CONFIG_CLK_ARM_1000_APLL_1000
///* APLL : 1.1GHz */
////#define CONFIG_CLK_ARM_1200_APLL_1100
///* APLL : 1.2GHz */
////#define CONFIG_CLK_ARM_1200_APLL_1200
///* APLL : 1.3GHz */
////#define CONFIG_CLK_ARM_1200_APLL_1300
///* APLL : 1.4GHz */
//#define CONFIG_CLK_ARM_1200_APLL_1400
///* APLL : 1.5GHz */
////#define CONFIG_CLK_ARM_1500_APLL_1500
//
///* bus clock: 100Mhz, DMC clock 200Mhz */
////#define CONFIG_CLK_BUS_DMC_100_200
///* bus clock: 165Mhz, DMC clock 330Mhz */
////#define CONFIG_CLK_BUS_DMC_165_330
///* bus clock: 200Mhz, DMC clock 400Mhz */
//#define CONFIG_CLK_BUS_DMC_200_400
//
///* IV_SIZE: 128 byte, 2 port(1 Gbyte), open page, trrd: 4 */
//#define CONFIG_EVT0_PERFORMANCE
///* IV_SIZE: 512 Mbyte, 1 port(512 Mbyte), open page, trrd: 4 */
////#define CONFIG_EVT0_STABLE
///* IV_SIZE: 128 byte, 2 port(1 Gbyte), close page, trrd: 0xA */
////#define CONFIG_EVT0_RECOMMEND
//
///* (Memory Interleaving Size = 1 << IV_SIZE) */
//#if defined(CONFIG_TINY4412A)
//#define CONFIG_IV_SIZE 0x1F
//#elif defined(CONFIG_EVT0_STABLE)
//#define CONFIG_IV_SIZE 0x1D
//#else
//#define CONFIG_IV_SIZE 0x7
//#endif
//
///* Notice for MSHC[Using of MMC CH4] */
///*
// * If you want MSHC at MMC CH4.
// */
//
//#define CONFIG_L2_OFF
//
////#define CONFIG_ARCH_CPU_INIT
//
//#define CONFIG_DISPLAY_CPUINFO
//#define CONFIG_DISPLAY_BOARDINFO
//#define CONFIG_BOARD_NAME	"TINY4412"
//#define BOARD_LATE_INIT
//
///* input clock of PLL: SMDKV310 has 24MHz input clock */
//#define CONFIG_SYS_CLK_FREQ	24000000
//
///* DRAM Base */
//#define CONFIG_SYS_SDRAM_BASE		0x40000000
//
//#define CONFIG_SETUP_MEMORY_TAGS
//#define CONFIG_CMDLINE_TAG
//#define CONFIG_INITRD_TAG
//#define CONFIG_CMDLINE_EDITING
//
///* Power Management is enabled */
//#define CONFIG_PM
//#define CONFIG_PM_VDD_ARM	1.2
//#define CONFIG_PM_VDD_INT	1.0
//#define CONFIG_PM_VDD_G3D	1.1
//#define CONFIG_PM_VDD_MIF	1.1
//#define CONFIG_PM_VDD_LDO14	1.8
//
///* Disable C2C or not */
//#undef CONFIG_C2C
//
///* NO PMIC on my board */
//#undef CONFIG_HAS_PMIC
//
///*
// * Size of malloc() pool
// * 1MB = 0x100000, 0x100000 = 1024 * 1024
// */
//#define CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + (1 << 20))
//									/* initial data */
//
///*
// * select serial console configuration
// */
//#define CONFIG_SERIAL0			1
//#define CONFIG_SERIAL_MULTI		1
//#undef CONFIG_LL_DEBUG				/* disable debug */
//
//#ifdef CONFIG_LL_DEBUG
//#define CONFIG_RAM_TEST			1	/* Memory R/W test */
//#endif
//
//#define CONFIG_USB_OHCI
//#undef CONFIG_USB_STORAGE
//#define CONFIG_S3C_USBD
//
//#define USBD_DOWN_ADDR		0xc0000000
//
///* allow to overwrite serial and ethaddr */
//#define CONFIG_ENV_OVERWRITE
//#define CONFIG_BAUDRATE			115200
//
///***********************************************************
// * Command definition
// ***********************************************************/
//#include <config_cmd_default.h>
//
//#define CONFIG_CMD_PING
//
//#define CONFIG_CMD_USB
//
//#define CONFIG_CMD_MOVINAND
//
//#undef CONFIG_CMD_FLASH
//#undef CONFIG_CMD_IMLS
//#ifndef CONFIG_TRUSTZONE
////#define CONFIG_CMD_NAND
//#endif
//
//#define CONFIG_CMD_CACHE
//#define CONFIG_CMD_REGINFO
//#define CONFIG_CMD_MMC
//#define CONFIG_CMD_MOVI
//#define CONFIG_CMD_ELF
//#define CONFIG_CMD_FAT
//#define CONFIG_CMD_MTDPARTS
//#define CONFIG_MTD_DEVICE
//
//#define CONFIG_CMD_EXT2
//#define CONFIG_CMD_FAT
//
//#define CONFIG_SYS_NAND_QUIET_TEST
//#define CONFIG_SYS_ONENAND_QUIET_TEST
//
//#define CONFIG_MMC
//#define CONFIG_GENERIC_MMC
//#define CONFIG_S3C_HSMMC
///* The macro for MMC channel 0 is defined by default and can't be undefined */
//
//#define MMC_MAX_CHANNEL		5
//
//#define USE_MMC2
//#define USE_MMC4
//
///*
// * BOOTP options
// */
//#define CONFIG_BOOTP_SUBNETMASK
//#define CONFIG_BOOTP_GATEWAY
//#define CONFIG_BOOTP_HOSTNAME
//#define CONFIG_BOOTP_BOOTPATH
//
//#define CONFIG_ETHADDR		00:40:5c:26:0a:5b
//#define CONFIG_NETMASK          255.255.255.0
//#define CONFIG_IPADDR		192.168.0.20
//#define CONFIG_SERVERIP		192.168.0.10
//#define CONFIG_GATEWAYIP	192.168.0.1
//
//#define CONFIG_BOOTDELAY	3
///* Default boot commands for Android booting. */
//#define CONFIG_BOOTCOMMAND	"movi read kernel 0 40008000;movi read rootfs 0 41000000 100000;bootm 40008000 41000000"
//#define CONFIG_BOOTARGS	""
//
//#define CONFIG_BOOTCOMMAND2	\
//		"mmc erase user 0 1072 1;"	\
//		"movi r f 1 40000000;emmc open 0;movi w z f 0 40000000;emmc close 0;"	\
//		"movi r u 1 40000000;emmc open 0;movi w z u 0 40000000;emmc close 0;"	\
//		"movi r k 1 40000000;movi w k 0 40000000;"				\
//		"movi r r 1 40000000 100000;movi w r 0 40000000 100000;"		\
//		"fdisk -c 0;"								\
//		"movi init 0;"								\
//		"fatformat mmc 0:1;"							\
//		"mmc read 1 48000000 20000 a0000;"					\
//		"fastboot flash system 48000000;"					\
//		"mmc read 1 48000000 c0000 a0000;"					\
//		"fastboot flash userdata 48000000;"					\
//		"mmc read 1 48000000 160000 a0000;"					\
//		"fastboot flash cache 48000000;"					\
//		"reset"
//
///*
// * Miscellaneous configurable options
// */
//#define CONFIG_SYS_LONGHELP		/* undef to save memory */
//#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
//#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
//#define CONFIG_SYS_PROMPT		"TINY4412 # "
//#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
//#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
//#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
///* Boot Argument Buffer Size */
//#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
///* memtest works on */
//#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
//#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5e00000)
//
//#define CONFIG_SYS_HZ			1000
//
///* valid baudrates */
//#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
//
///*-----------------------------------------------------------------------
// * Stack sizes
// *
// * The stack sizes are set up in start.S using the settings below
// */
//#define CONFIG_STACKSIZE	(256 << 10)	/* 256 KiB */
//
//
//#ifdef CONFIG_EVT0_STABLE
//#define CONFIG_NR_DRAM_BANKS	2
//#else
//#define CONFIG_NR_DRAM_BANKS	4
//#endif
//#define SDRAM_BANK_SIZE         0x10000000    /* 256 MB */
//#define PHYS_SDRAM_1            CONFIG_SYS_SDRAM_BASE /* SDRAM Bank #1 */
//#define PHYS_SDRAM_1_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_2            (CONFIG_SYS_SDRAM_BASE + SDRAM_BANK_SIZE) /* SDRAM Bank #2 */
//#define PHYS_SDRAM_2_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_3            (CONFIG_SYS_SDRAM_BASE + 2 * SDRAM_BANK_SIZE) /* SDRAM Bank #3 */
//#define PHYS_SDRAM_3_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_4            (CONFIG_SYS_SDRAM_BASE + 3 * SDRAM_BANK_SIZE) /* SDRAM Bank #4 */
//#define PHYS_SDRAM_4_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_5            (CONFIG_SYS_SDRAM_BASE + 4 * SDRAM_BANK_SIZE) /* SDRAM Bank #5 */
//#define PHYS_SDRAM_5_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_6            (CONFIG_SYS_SDRAM_BASE + 5 * SDRAM_BANK_SIZE) /* SDRAM Bank #6 */
//#define PHYS_SDRAM_6_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_7            (CONFIG_SYS_SDRAM_BASE + 6 * SDRAM_BANK_SIZE) /* SDRAM Bank #7 */
//#define PHYS_SDRAM_7_SIZE       SDRAM_BANK_SIZE
//#define PHYS_SDRAM_8            (CONFIG_SYS_SDRAM_BASE + 7 * SDRAM_BANK_SIZE) /* SDRAM Bank #8 */
//#define PHYS_SDRAM_8_SIZE       SDRAM_BANK_SIZE
//
//#define CONFIG_SYS_MONITOR_BASE	0x00000000
//
///*-----------------------------------------------------------------------
// * FLASH and environment organization
// */
//#define CONFIG_SYS_NO_FLASH		1
//
//#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 KiB */
//#define CONFIG_IDENT_STRING		" for TINY4412"
//
//#define CONFIG_ENABLE_MMU
//
//#ifdef CONFIG_ENABLE_MMU
//#define CONFIG_SYS_MAPPED_RAM_BASE	0xc0000000
//#define virt_to_phys(x)	virt_to_phy_s5pv310(x)
//#else
//#define CONFIG_SYS_MAPPED_RAM_BASE	CONFIG_SYS_SDRAM_BASE
//#define virt_to_phys(x)	(x)
//#endif
//
//#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_MAPPED_RAM_BASE + 0x3e00000
//#define CONFIG_PHY_UBOOT_BASE		CONFIG_SYS_SDRAM_BASE + 0x3e00000
//
///*
// *  Fast Boot
//*/
///* Fastboot variables */
//#define CFG_FASTBOOT_TRANSFER_BUFFER            (0x48000000)
//#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE       (0x18000000)   /* 384MB */
//#define CFG_FASTBOOT_ADDR_KERNEL                (0x40008000)
//#define CFG_FASTBOOT_ADDR_RAMDISK               (0x41000000)
//#define CFG_FASTBOOT_PAGESIZE                   (2048)  // Page size of booting device
//#define CFG_FASTBOOT_SDMMC_BLOCKSIZE            (512)   // Block size of sdmmc
//#define CFG_PARTITION_START			(0x4000000)
//
///* Just one BSP type should be defined. */
//#if defined(CONFIG_CMD_ONENAND) | defined(CONFIG_CMD_NAND) | defined(CONFIG_CMD_MOVINAND)
//#define CONFIG_FASTBOOT
//#endif
//
//#if defined(CONFIG_CMD_NAND)
//#define CFG_FASTBOOT_NANDBSP
//#endif
//#if defined(CONFIG_CMD_ONENAND)
//#define CFG_FASTBOOT_ONENANDBSP
//#endif
//#if defined(CONFIG_CMD_MOVINAND)
//#define CFG_FASTBOOT_SDMMCBSP
//#endif
//
///*
// * machine type
// */
//
//#define MACH_TYPE_TINY4412		4608	/* Tiny4412 machine ID */
//
//#define CONFIG_ENV_OFFSET		0x0007C000
//
///*-----------------------------------------------------------------------
// * Boot configuration
// */
//#define CONFIG_FASTBOOT
//
//#define BOOT_ONENAND		0x1
//#define BOOT_NAND		0x40000
//#define BOOT_MMCSD		0x3
//#define BOOT_NOR		0x4
//#define BOOT_SEC_DEV		0x5
//#define BOOT_EMMC		0x6
//#define BOOT_EMMC_4_4		0x7
//
///* nand copy size from nand to DRAM.*/
//#define	COPY_BL2_SIZE		0x80000
//#define CONFIG_SYS_MAX_NAND_DEVICE     1
//#define CFG_NAND_SKIP_BAD_DOT_I	1  /* ".i" read skips bad blocks   */
//#define	CFG_NAND_WP		1
//#define CFG_NAND_YAFFS_WRITE	1  /* support yaffs write */
//#define CFG_NAND_HWECC
//#define CONFIG_NAND_BL1_16BIT_ECC
//#undef	CFG_NAND_FLASH_BBT
//
//#define CONFIG_SYS_NAND_BASE           (0x0CE00000)
//#define NAND_MAX_CHIPS          1
//
//#define NAND_DISABLE_CE()	(NFCONT_REG |= (1 << 1))
//#define NAND_ENABLE_CE()	(NFCONT_REG &= ~(1 << 1))
//#define NF_TRANSRnB()		do { while(!(NFSTAT_REG & (1 << 0))); } while(0)
//
//#define CFG_NAND_SKIP_BAD_DOT_I	1  /* ".i" read skips bad blocks   */
//#define	CFG_NAND_WP		1
//#define CFG_NAND_YAFFS_WRITE	1  /* support yaffs write */
//
//#define CFG_NAND_HWECC
//#define CONFIG_NAND_BL1_16BIT_ECC
//#undef	CFG_NAND_FLASH_BBT
//
//#define CONFIG_ZIMAGE_BOOT
//
//#define CONFIG_ENV_IS_IN_AUTO		1
//#define CONFIG_ENV_SIZE			0x4000
//
//#define CONFIG_DOS_PARTITION		1
//
////#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - 0x1000000)
//#define CONFIG_SYS_INIT_SP_ADDR	(0x43e00000 - 0x1000000)
//
///*
// * Ethernet Contoller driver
// */
//#ifdef CONFIG_CMD_NET
//#define CONFIG_NET_MULTI
//
///* NO SMC911X on Tiny4412 */
//#ifndef CONFIG_TINY4412A
//#define CONFIG_SMC911X
//#define CONFIG_SMC911X_BASE	0x5000000
//#define CONFIG_SMC911X_16_BIT
//#endif
//#endif /* CONFIG_CMD_NET */
//
///* GPIO */
//#define GPIO_BASE	0x11000000
//
//#define CONFIG_SAMSUNG_ONENAND		1
//
//#if defined(CONFIG_CMD_ONENAND)
//  #define CONFIG_C210_ONENAND
//#endif
//
//#define CFG_ONENAND_BASE 	(0x0C000000)
//#define CFG_ONENANDXL_BASE 	(0x0C600000)
//#define CONFIG_SYS_ONENAND_BASE	CFG_ONENAND_BASE
//#define CONFIG_SYS_MAX_ONENAND_DEVICE     1
//
//#define CONFIG_BOOT_ONENAND_IROM
//#define CONFIG_NAND
//#define CONFIG_BOOT_NAND
//
//#define CFG_PHY_UBOOT_BASE	MEMORY_BASE_ADDRESS + 0x3e00000
//#define CFG_PHY_KERNEL_BASE	MEMORY_BASE_ADDRESS + 0x8000
//
//#define MEMORY_BASE_ADDRESS	0x40000000
//
//#endif	/* __CONFIG_H */
