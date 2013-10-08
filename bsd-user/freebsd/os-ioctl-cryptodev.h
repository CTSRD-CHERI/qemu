/*
 *  FreeBSD cryptodev definitions for ioctl(2) emulation
 *
 *  Copyright (c) 2014 Stacey D. Son
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _IOCTL_CRYPTODEV_H_
#define _IOCTL_CRYPTODEV_H_

/* see opencrypto/cryptodev.h */

struct target_session_op {
	u_int32_t	cipher;
	u_int32_t	mac;
	
	u_int32_t	keylen;
	abi_ulong	key;
	int32_t		mackeylen;
	abi_ulong	mackey;

	u_int32_t	ses;
};


struct target_session2_op {
	u_int32_t	cipher;
	u_int32_t	mac;
	
	u_int32_t	keylen;
	abi_ulong	key;
	int32_t		mackeylen;
	abi_ulong	mackey;

	u_int32_t	ses;
	int32_t		crid;
	int		pad[4];
};

struct target_crypt_find_op {
	int		crid;
        char		name[32];
};

struct target_crparam {
	abi_ulong	crp_p;
        u_int		crp_nbits;
};

#define	TARGET_CRK_MAXPARAM	8 

struct target_crypt_kop {
	u_int		crk_op;
        u_int		crk_status;
        u_short		crk_iparams;
        u_short		crk_oparams;
        u_int		crk_crid;
        struct target_crparam	crk_param[TARGET_CRK_MAXPARAM];
};

#define	TARGET_CRIOGET		TARGET_IOWR('c', 100, u_int32_t)
#define TARGET_CRIOASYMFEAT	TARGET_CIOCASYMFEAT
#define	TARGET_CRIOFINDDEV	TARGET_CIOCFINDDEV

#define	TARGET_CIOCGSESSION	TARGET_IOWR('c', 101, struct target_session_op)
#define	TARGET_CIOCFSESSION	TARGET_IOW('c', 102, u_int32_t)
#define	TARGET_CIOCCRYPT	TARGET_IOWR('c', 103, struct target_crypt_op)
#define	TARGET_CIOCKEY		TARGET_IOWR('c', 104, struct target_crypt_kop)
#define	TARGET_CIOCASYMFEAT	TARGET_IOR('c', 105, u_int32_t)
#define	TARGET_CIOCGSESSION2	TARGET_IOWR('c', 106, struct target_session2_op)
#define	TARGET_CIOCKEY2		TARGET_IOWR('c', 107, struct target_crypt_kop)
#define	TARGET_CIOCFINDDEV	TARGET_IOWR('c', 108, struct target_crypt_find_op)

#endif /* !_IOCTL_CRYPTODEV_H_ */
