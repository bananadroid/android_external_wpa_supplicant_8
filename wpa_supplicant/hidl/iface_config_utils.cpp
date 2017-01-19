/*
 * hidl interface for wpa_supplicant daemon
 * Copyright (struct wpa_supplicant* wpa_s, c) 2004-2016, Jouni Malinen
 * <j@w1.fi>
 * Copyright (struct wpa_supplicant* wpa_s, c) 2004-2016, Roshan Pius
 * <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "hidl_manager.h"
#include "hidl_return_util.h"
#include "iface_config_utils.h"

namespace {
constexpr uint32_t kMaxWpsDeviceNameSize = WPS_DEV_NAME_MAX_LEN;
constexpr uint32_t kMaxWpsManufacturerSize = 64;
constexpr uint32_t kMaxWpsModelNameSize = 32;
constexpr uint32_t kMaxWpsModelNumberSize = 32;
constexpr uint32_t kMaxWpsSerialNumberSize = 32;

// Free any existing pointer stored in |dst| and store the provided string value
// there.
int freeAndSetStringConfigParam(
    wpa_supplicant* wpa_s, const std::string& value, uint32_t max_size,
    uint32_t changed_param, char** dst)
{
	if (value.size() > max_size) {
		return -1;
	}
	WPA_ASSERT(dst);
	os_free(static_cast<void*>(*dst));
	*dst = os_strdup(value.c_str());
	wpa_s->conf->changed_parameters |= changed_param;
	wpa_supplicant_update_config(wpa_s);
	return 0;
}

std::string convertWpsConfigMethodsMaskToString(uint16_t config_methods)
{
	using WpsConfigMethods =
	    android::hardware::wifi::supplicant::V1_0::WpsConfigMethods;
	std::string config_methods_str;
	for (const auto& flag_and_name :
	     {std::make_pair(WpsConfigMethods::USBA, "usba"),
	      {WpsConfigMethods::ETHERNET, "ethernet"},
	      {WpsConfigMethods::LABEL, "label"},
	      {WpsConfigMethods::DISPLAY, "display"},
	      {WpsConfigMethods::INT_NFC_TOKEN, "int_nfc_token"},
	      {WpsConfigMethods::EXT_NFC_TOKEN, "ext_nfc_token"},
	      {WpsConfigMethods::NFC_INTERFACE, "nfc_interface"},
	      {WpsConfigMethods::PUSHBUTTON, "pushbutton"},
	      {WpsConfigMethods::KEYPAD, "keypad"},
	      {WpsConfigMethods::VIRT_PUSHBUTTON, "virt_pushbutton"},
	      {WpsConfigMethods::PHY_PUSHBUTTON, "phy_pushbutton"},
	      {WpsConfigMethods::P2PS, "p2ps"},
	      {WpsConfigMethods::VIRT_DISPLAY, "virt_display"},
	      {WpsConfigMethods::PHY_DISPLAY, "phy_display"}}) {
		if (config_methods & flag_and_name.first) {
			config_methods_str += flag_and_name.second;
			config_methods_str += " ";
		}
	}
	return config_methods_str;
}
}  // namespace

namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
namespace V1_0 {
namespace implementation {
namespace iface_config_utils {
SupplicantStatus setWpsDeviceName(
    struct wpa_supplicant* wpa_s, const std::string& name)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, name, kMaxWpsDeviceNameSize, CFG_CHANGED_DEVICE_NAME,
		&wpa_s->conf->device_name)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setWpsManufacturer(
    struct wpa_supplicant* wpa_s, const std::string& manufacturer)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, manufacturer, kMaxWpsManufacturerSize,
		CFG_CHANGED_WPS_STRING, &wpa_s->conf->manufacturer)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setWpsModelName(
    struct wpa_supplicant* wpa_s, const std::string& model_name)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, model_name, kMaxWpsModelNameSize, CFG_CHANGED_WPS_STRING,
		&wpa_s->conf->model_name)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setWpsModelNumber(
    struct wpa_supplicant* wpa_s, const std::string& model_number)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, model_number, kMaxWpsModelNumberSize,
		CFG_CHANGED_WPS_STRING, &wpa_s->conf->model_number)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setWpsSerialNumber(
    struct wpa_supplicant* wpa_s, const std::string& serial_number)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, serial_number, kMaxWpsSerialNumberSize,
		CFG_CHANGED_WPS_STRING, &wpa_s->conf->serial_number)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setWpsConfigMethods(
    struct wpa_supplicant* wpa_s, uint16_t config_methods)
{
	WPA_ASSERT(wpa_s);
	if (freeAndSetStringConfigParam(
		wpa_s, convertWpsConfigMethodsMaskToString(config_methods),
		UINT32_MAX, CFG_CHANGED_CONFIG_METHODS,
		&wpa_s->conf->config_methods)) {
		return {SupplicantStatusCode::FAILURE_ARGS_INVALID, ""};
	}
	return {SupplicantStatusCode::SUCCESS, ""};
}

SupplicantStatus setExternalSim(
    struct wpa_supplicant* wpa_s, bool useExternalSim)
{
	WPA_ASSERT(wpa_s);
	wpa_s->conf->external_sim = useExternalSim ? 1 : 0;
	return {SupplicantStatusCode::SUCCESS, ""};
}
}  // namespace iface_config_utils
}  // namespace implementation
}  // namespace V1_0
}  // namespace wifi
}  // namespace supplicant
}  // namespace hardware
}  // namespace android
