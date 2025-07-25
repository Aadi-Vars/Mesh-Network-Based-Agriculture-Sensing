/**
 * Copyright (c) 2018 - 2022, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef ZBOSS_BUILD_INFO_H__
#define ZBOSS_BUILD_INFO_H__

/**
 * This is an autogenerated file.
 * Please use release scripts to update it's contents.
 *
 * Repositories:
 * platform_ncs       1ce984f7be5486574694e6b65401150b92bfb6b7 https://projecttools.nordicsemi.no/bitbucket/scm/ZOI/platform_ncs.git
 * zboss-stack-src    77d61ff26c5782e66f0f16a85e24ce8b79901f54 https://projecttools.nordicsemi.no/bitbucket/scm/zoi/zboss_stable
 * zboss-stack-doc    c41a9102bf0140bf9a5d7186ee157a9ccfd7e870 https://projecttools.nordicsemi.no/bitbucket/scm/zoi/zboss_doc
 * nrf                7b0f72fd0bd58336916245aa40c0006d4331980c https://github.com/nrfconnect/sdk-nrf
 * zephyr             d9cc0d37bf45a544ccc7bb0df159d470b836724d https://github.com/nrfconnect/sdk-zephyr
 * mcuboot            296af282cc269f4983dc6814180109e5f5808a1a https://github.com/nrfconnect/sdk-mcuboot
 * mbedtls-nrf        68dfba636301bd618bc9b5c1a946f358ad8714ee https://github.com/nrfconnect/sdk-mbedtls
 * nrfxlib            943bf88606f0df8b705e38182c10a8cf959dba45 https://github.com/nrfconnect/sdk-nrfxlib
 * trusted-firmware-m b599a2e3b93b256ccb886d3368ab9948845ebac4 https://github.com/nrfconnect/sdk-trusted-firmware-m
 * tfm-mcuboot        4aa516e7281fc6f9a2dce0b0efda9acc580fa254 https://github.com/nrfconnect/sdk-mcuboot
 * matter             c0da86c97e5761dfbc828ab76ef18150b7215f23 https://github.com/nrfconnect/sdk-connectedhomeip
 * cjson              c6af068b7f05207b28d68880740e4b9ec1e4b50a https://github.com/nrfconnect/sdk-cjson
 * cmock              9d092898ef26ece140d9225e037274b64d4f851e https://github.com/ThrowTheSwitch/cmock
 * unity              74cde089e65c3435ce9aa87d5c69f4f16b7f6ade https://github.com/ThrowTheSwitch/unity
 * Alexa-Gadgets-Embedded-Sample-Code face92d8c62184832793f518bb1f19379538c5c1 https://github.com/alexa/Alexa-Gadgets-Embedded-Sample-Code
 * cddl-gen           1c2656daa87ebf4cd3c45ee8eafa336260d589bc https://github.com/NordicSemiconductor/cddl-gen
 * memfault-firmware-sdk 8585f81453af37cd97c47816cc61395cbabdf1fe https://github.com/memfault/memfault-firmware-sdk
 * canopennode        1052dae561497bef901f931ef75e117c9224aecd https://github.com/zephyrproject-rtos/canopennode
 * civetweb           094aeb41bb93e9199d24d665ee43e9e05d6d7b1c https://github.com/zephyrproject-rtos/civetweb
 * cmsis              b0612c97c1401feeb4160add6462c3627fe90fc7 https://github.com/zephyrproject-rtos/cmsis
 * edtt               c94f63865a803b18a2ab9eca70d2a9ab881130f6 https://github.com/zephyrproject-rtos/edtt
 * fatfs              94fcd6bfb3801ac0a5e12ea2f52187e0a688b90e https://github.com/zephyrproject-rtos/fatfs
 * hal_nordic         a42b016d7c7610489f5f8c79773fedc05ba352ee https://github.com/zephyrproject-rtos/hal_nordic
 * hal_st             575de9d461aa6f430cf62c58a053675377e700f3 https://github.com/zephyrproject-rtos/hal_st
 * libmetal           f237c9d420a51cc43bc37d744e41191ad613f348 https://github.com/zephyrproject-rtos/libmetal
 * littlefs           33509ed9c3d369cdb9d909cd40c5eea8f64a902c https://github.com/zephyrproject-rtos/littlefs
 * loramac-node       12019623bbad9eb54fe51066847a7cbd4b4eac57 https://github.com/zephyrproject-rtos/loramac-node
 * lvgl               783c1f78c8e39751fe89d0883c8bce7336f55e94 https://github.com/zephyrproject-rtos/lvgl
 * lz4                8e303c264fc21c2116dc612658003a22e933124d https://github.com/zephyrproject-rtos/lz4
 * mbedtls            3e3e58a92de76069730c63e0d524f40fea948a61 https://github.com/zephyrproject-rtos/mbedtls
 * mipi-sys-t         75e671550ac1acb502f315fe4952514dc73f7bfb https://github.com/zephyrproject-rtos/mipi-sys-t
 * nanopb             d148bd26718e4c10414f07a7eb1bd24c62e56c5d https://github.com/zephyrproject-rtos/nanopb
 * net-tools          f49bd1354616fae4093bf36e5eaee43c51a55127 https://github.com/zephyrproject-rtos/net-tools
 * nrf_hw_models      b8cea37dbdc8fc58cc14b4e19fa850877a9da520 https://github.com/zephyrproject-rtos/nrf_hw_models
 * open-amp           cfd050ff38a9d028dc211690b2ec35971128e45e https://github.com/zephyrproject-rtos/open-amp
 * openthread         41bc49da49736fbdfdfa231f7b2311f29dcc4979 https://github.com/zephyrproject-rtos/openthread
 * segger             3a52ab222133193802d3c3b4d21730b9b1f1d2f6 https://github.com/zephyrproject-rtos/segger
 * tinycbor           40daca97b478989884bffb5226e9ab73ca54b8c4 https://github.com/zephyrproject-rtos/tinycbor
 * tinycrypt          3e9a49d2672ec01435ffbf0d788db6d95ef28de0 https://github.com/zephyrproject-rtos/tinycrypt
 * TraceRecorderSource 36c577727642457b0db7274298a4b96558374832 https://github.com/zephyrproject-rtos/TraceRecorderSource
 * tf-m-tests         52814181f0fde6d1422fac204d42cde30c62e40e https://github.com/zephyrproject-rtos/tf-m-tests
 * psa-arch-tests     186cba2543dff73d0cda5509d26f02a0b39ee66e https://github.com/zephyrproject-rtos/psa-arch-tests
 * zscilib            12bfe3f0a9fcbfe3edab7eabc9678b6c62875d34 https://github.com/zephyrproject-rtos/zscilib
 */


/** ZBOSS build tag */
#define ZBOSS_BUILD_TAG "nrf5_sdk_release_3.11.1.5"

/** ZBOSS platform build tag */
#define ZBOSS_PLATFORM_BUILD_TAG "v3.11.1.5+v5.1.0"

/** ZBOSS build date (UTC) */
#define ZBOSS_BUILD_DATE 20220304

/** ZBOSS build time (UTC) */
#define ZBOSS_BUILD_TIME 161555


#endif /* ZBOSS_BUILD_INFO_H__ */
