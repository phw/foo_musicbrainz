#include "stdafx.h"

namespace mb_preferences
{
	const GUID guid_server = { 0x2ac00b3b, 0x1b04, 0x4fb2,{ 0xa9, 0x98, 0x5c, 0x16, 0x4, 0x9c, 0xce, 0x9d } };
	const bool default_server = false;
	cfg_bool server(guid_server, default_server);

	const GUID guid_short_date = { 0x18734618, 0x7920, 0x4d24,{ 0x84, 0xa1, 0xb9, 0xd6, 0x6e, 0xd8, 0x25, 0xbc } };
	const bool default_short_date = false;
	cfg_bool short_date(guid_short_date, default_short_date);

	const GUID guid_ascii_punctuation = { 0xd08b1b7c, 0x38fd, 0x4689,{ 0x9e, 0x91, 0x8c, 0xdc, 0xbe, 0xc4, 0x26, 0x98 } };
	const bool default_ascii_punctuation = false;
	cfg_bool ascii_punctuation(guid_ascii_punctuation, default_ascii_punctuation);

	const GUID guid_write_ids = { 0x8c8b61e0, 0x8eea, 0x4c34,{ 0x9a, 0x51, 0x4d, 0xa9, 0x9c, 0xec, 0xcb, 0xbc } };
	const bool default_write_ids = true;
	cfg_bool write_ids(guid_write_ids, default_write_ids);

	const GUID guid_albumtype = { 0xd8fdb1d8, 0xde2, 0x4f1f,{ 0x85, 0x5a, 0xc0, 0x5, 0x98, 0x60, 0x9b, 0xe9 } };
	const bool default_albumtype = true;
	cfg_bool albumtype(guid_albumtype, default_albumtype);

	const GUID guid_albumstatus = { 0x57a8dddd, 0xdf24, 0x4fd3,{ 0xac, 0x95, 0x6f, 0x8, 0x3, 0x26, 0x41, 0x7c } };
	const bool default_albumstatus = true;
	cfg_bool albumstatus(guid_albumstatus, default_albumstatus);

	const GUID guid_write_label_info = { 0x9b3c94e3, 0x278, 0x4eb0,{ 0xa2, 0xed, 0x5, 0xad, 0xf8, 0xce, 0xa3, 0x9d } };
	const bool default_write_label_info = true;
	cfg_bool write_label_info(guid_write_label_info, default_write_label_info);

	const GUID guid_write_country = { 0xa983fbd6, 0x7471, 0x41d7,{ 0xa7, 0x62, 0x27, 0xf9, 0x94, 0xe9, 0x23, 0x1f } };
	const bool default_write_country = true;
	cfg_bool write_country(guid_write_country, default_write_country);

	const GUID guid_write_format = { 0x8e0f77e2, 0x50e8, 0x4e66,{ 0x88, 0xe3, 0xab, 0xed, 0x3b, 0x76, 0x84, 0x5a } };
	const bool default_write_format = true;
	cfg_bool write_format(guid_write_format, default_write_format);

	const GUID guid_write_asin = { 0x6677d957, 0xaa52, 0x4fef, { 0x95, 0x81, 0xb3, 0xe3, 0x20, 0x16, 0xaf, 0x4d } };
	const bool default_write_asin = true;
	cfg_bool write_asin(guid_write_asin, default_write_asin);

	const GUID guid_write_isrc = { 0x8b129571, 0x1bf2, 0x41bd, { 0x86, 0xf1, 0xcd, 0xec, 0x6, 0xb9, 0xf3, 0xff } };
	const bool default_write_isrc = true;
	cfg_bool write_isrc(guid_write_isrc, default_write_isrc);

	const GUID guid_write_albumartist = { 0x5cbeb3ba, 0xae9f, 0x4975, { 0xb9, 0x94, 0xd7, 0x50, 0xda, 0x2b, 0x22, 0x0 } };
	const bool default_write_albumartist = false;
	cfg_bool write_albumartist(guid_write_albumartist, default_write_albumartist);

	const GUID guid_server_data = { 0x8732cde8, 0xe1ed, 0x4824,{ 0xae, 0xf6, 0x7d, 0x4b, 0x56, 0x99, 0x23, 0xac } };
	cfg_string server_data(guid_server_data, "https://musicbrainz.org");

	const GUID guid_albumtype_data = { 0x20968c09, 0xb0d4, 0x4059,{ 0xb8, 0x92, 0xda, 0x76, 0xf8, 0xe6, 0x51, 0x4e } };
	cfg_string albumtype_data(guid_albumtype_data, "MUSICBRAINZ_ALBUMTYPE");

	const GUID guid_albumstatus_data = { 0x77182aac, 0x1caa, 0x4793,{ 0xb7, 0x15, 0xcc, 0xf8, 0x97, 0xba, 0x11, 0x1a } };
	cfg_string albumstatus_data(guid_albumstatus_data, "MUSICBRAINZ_ALBUMSTATUS");
}

class PreferencesPageInstance : public CDialogImpl<PreferencesPageInstance>, public preferences_page_instance
{
public:
	PreferencesPageInstance(preferences_page_callback::ptr p_callback) : m_callback(p_callback) {}

	enum { IDD = IDD_PREFERENCES };

	BEGIN_MSG_MAP(PreferencesPageInstance)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_CODE_HANDLER_EX(EN_UPDATE, OnChanged)
		COMMAND_RANGE_HANDLER_EX(IDC_SERVER, IDC_WRITE_ALBUMARTIST, OnChanged)
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow, LPARAM)
	{
		server_checkbox = GetDlgItem(IDC_SERVER);
		short_date_checkbox = GetDlgItem(IDC_SHORT_DATE);
		ascii_punctuation_checkbox = GetDlgItem(IDC_ASCII_PUNCTUATION);
		write_ids_checkbox = GetDlgItem(IDC_WRITE_IDS);
		write_albumtype_checkbox = GetDlgItem(IDC_ALBUMTYPE);
		write_albumstatus_checkbox = GetDlgItem(IDC_ALBUMSTATUS);
		write_label_info_checkbox = GetDlgItem(IDC_WRITE_LABEL_INFO);
		write_country_checkbox = GetDlgItem(IDC_WRITE_COUNTRY);
		write_format_checkbox = GetDlgItem(IDC_WRITE_FORMAT);
		write_asin_checkbox = GetDlgItem(IDC_WRITE_ASIN);
		write_isrc_checkbox = GetDlgItem(IDC_WRITE_ISRC);
		write_albumartist_checkbox = GetDlgItem(IDC_WRITE_ALBUMARTIST);

		server_data = GetDlgItem(IDC_SERVER_DATA);
		albumtype_data = GetDlgItem(IDC_ALBUMTYPE_DATA);
		albumstatus_data = GetDlgItem(IDC_ALBUMSTATUS_DATA);
		
		server_checkbox.SetCheck(mb_preferences::server);
		short_date_checkbox.SetCheck(mb_preferences::short_date);
		ascii_punctuation_checkbox.SetCheck(mb_preferences::ascii_punctuation);
		write_ids_checkbox.SetCheck(mb_preferences::write_ids);
		write_albumtype_checkbox.SetCheck(mb_preferences::albumtype);
		write_albumstatus_checkbox.SetCheck(mb_preferences::albumstatus);
		write_label_info_checkbox.SetCheck(mb_preferences::write_label_info);
		write_country_checkbox.SetCheck(mb_preferences::write_country);
		write_format_checkbox.SetCheck(mb_preferences::write_format);
		write_asin_checkbox.SetCheck(mb_preferences::write_asin);
		write_isrc_checkbox.SetCheck(mb_preferences::write_isrc);
		write_albumartist_checkbox.SetCheck(mb_preferences::write_albumartist);

		server_data.EnableWindow(mb_preferences::server);
		albumtype_data.EnableWindow(mb_preferences::albumtype);
		albumstatus_data.EnableWindow(mb_preferences::albumstatus);

		uSetWindowText(server_data, mb_preferences::server_data);
		uSetWindowText(albumtype_data, mb_preferences::albumtype_data);
		uSetWindowText(albumstatus_data, mb_preferences::albumstatus_data);

		return FALSE;
	}

	bool has_changed()
	{
		if (server_checkbox.IsChecked() != mb_preferences::server) return true;
		if (short_date_checkbox.IsChecked() != mb_preferences::short_date) return true;
		if (ascii_punctuation_checkbox.IsChecked() != mb_preferences::ascii_punctuation) return true;
		if (write_ids_checkbox.IsChecked() != mb_preferences::write_ids) return true;
		if (write_albumtype_checkbox.IsChecked() != mb_preferences::albumtype) return true;
		if (write_albumstatus_checkbox.IsChecked() != mb_preferences::albumstatus) return true;
		if (write_label_info_checkbox.IsChecked() != mb_preferences::write_label_info) return true;
		if (write_country_checkbox.IsChecked() != mb_preferences::write_country) return true;
		if (write_format_checkbox.IsChecked() != mb_preferences::write_format) return true;
		if (write_asin_checkbox.IsChecked() != mb_preferences::write_asin) return true;
		if (write_isrc_checkbox.IsChecked() != mb_preferences::write_isrc) return true;
		if (write_albumartist_checkbox.IsChecked() != mb_preferences::write_albumartist) return true;

		str8 temp;
		uGetWindowText(server_data, temp);
		if (mb_preferences::server_data != temp) return true;
		uGetWindowText(albumtype_data, temp);
		if (mb_preferences::albumtype_data != temp) return true;
		uGetWindowText(albumstatus_data, temp);
		if (mb_preferences::albumstatus_data != temp) return true;

		return false;
	}

	t_uint32 get_state() override
	{
		t_uint32 state = preferences_state::resettable;
		if (has_changed()) state |= preferences_state::changed;
		return state;
	}

	void apply() override
	{
		mb_preferences::server = server_checkbox.IsChecked();
		mb_preferences::short_date = short_date_checkbox.IsChecked();
		mb_preferences::ascii_punctuation = ascii_punctuation_checkbox.IsChecked();
		mb_preferences::write_ids = write_ids_checkbox.IsChecked();
		mb_preferences::albumtype = write_albumtype_checkbox.IsChecked();
		mb_preferences::albumstatus = write_albumstatus_checkbox.IsChecked();
		mb_preferences::write_label_info = write_label_info_checkbox.IsChecked();
		mb_preferences::write_country = write_country_checkbox.IsChecked();
		mb_preferences::write_format = write_format_checkbox.IsChecked();
		mb_preferences::write_asin = write_asin_checkbox.IsChecked();
		mb_preferences::write_isrc = write_isrc_checkbox.IsChecked();
		mb_preferences::write_albumartist = write_albumartist_checkbox.IsChecked();

		uGetWindowText(server_data, mb_preferences::server_data);
		uGetWindowText(albumtype_data, mb_preferences::albumtype_data);
		uGetWindowText(albumstatus_data, mb_preferences::albumstatus_data);
	}

	void on_change()
	{
		m_callback->on_state_changed();
	}

	void reset() override
	{
		server_checkbox.SetCheck(mb_preferences::default_server);
		short_date_checkbox.SetCheck(mb_preferences::default_short_date);
		ascii_punctuation_checkbox.SetCheck(mb_preferences::default_ascii_punctuation);
		write_ids_checkbox.SetCheck(mb_preferences::default_write_ids);
		write_albumtype_checkbox.SetCheck(mb_preferences::default_albumtype);
		write_albumstatus_checkbox.SetCheck(mb_preferences::default_albumstatus);
		write_label_info_checkbox.SetCheck(mb_preferences::default_write_label_info);
		write_country_checkbox.SetCheck(mb_preferences::default_write_country);
		write_format_checkbox.SetCheck(mb_preferences::default_write_format);
		write_asin_checkbox.SetCheck(mb_preferences::default_write_asin);
		write_isrc_checkbox.SetCheck(mb_preferences::default_write_isrc);
		write_albumartist_checkbox.SetCheck(mb_preferences::default_write_albumartist);

		server_data.EnableWindow(mb_preferences::default_server);
		albumtype_data.EnableWindow(mb_preferences::default_albumtype);
		albumstatus_data.EnableWindow(mb_preferences::default_albumstatus);

		uSetWindowText(server_data, "https://musicbrainz.org");
		uSetWindowText(albumtype_data, "MUSICBRAINZ_ALBUMTYPE");
		uSetWindowText(albumstatus_data, "MUSICBRAINZ_ALBUMSTATUS");

		on_change();
	}

	void OnChanged(UINT, int, CWindow)
	{
		server_data.EnableWindow(server_checkbox.IsChecked());
		albumtype_data.EnableWindow(write_albumtype_checkbox.IsChecked());
		albumstatus_data.EnableWindow(write_albumstatus_checkbox.IsChecked());

		on_change();
	}

private:
	CCheckBox server_checkbox;
	CCheckBox short_date_checkbox;
	CCheckBox ascii_punctuation_checkbox;
	CCheckBox write_ids_checkbox;
	CCheckBox write_albumtype_checkbox;
	CCheckBox write_albumstatus_checkbox;
	CCheckBox write_label_info_checkbox;
	CCheckBox write_country_checkbox;
	CCheckBox write_format_checkbox;
	CCheckBox write_asin_checkbox;
	CCheckBox write_isrc_checkbox;
	CCheckBox write_albumartist_checkbox;
	CEdit server_data;
	CEdit albumtype_data;
	CEdit albumstatus_data;
	preferences_page_callback::ptr m_callback;
};

class PreferencesPage : public preferences_page_impl<PreferencesPageInstance>
{
public:
	const char* get_name() override
	{
		return COMPONENT_TITLE;
	}

	GUID get_guid() override
	{
		static const GUID guid = { 0x79179a37, 0x5942, 0x4fdf,{ 0xbb, 0xb7, 0x93, 0xfd, 0x35, 0xfc, 0xfe, 0x97 } };
		return guid;
	}

	GUID get_parent_guid() override
	{
		return preferences_page::guid_tagging;
	}
};

preferences_page_factory_t<PreferencesPage> _;
