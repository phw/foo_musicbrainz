#pragma once

#include "Query.h"
#include "TrackListView.h"
#include "TagWriter.h"
#include "ListView.h"
#include "RequestThread.h"
#include "Release.h"
#include "ReleaseGroup.h"
#include "Track.h"

namespace foo_musicbrainz {
	class TaggerDialog : public CDialogImpl<TaggerDialog> {
	private:
		CListViewCtrl release_list;
		ListView medium_list;
		CListViewCtrl track_list;
		CComboBox type;
		CComboBox status;
		CEdit artist;
		CEdit album;
		CEdit date;
		CEdit disc;
		CEdit discsubtitle;
		CEdit url;
		ReleaseList *mbc;
		pfc::list_t<metadb_handle_ptr> tracks;
		TrackListView track_list_view;
		bool loaded;
		size_t current_release;
		size_t current_disc;

	public:
		enum { IDD = IDD_TAGGER };

		TaggerDialog(foo_musicbrainz::Query *query, pfc::list_t<metadb_handle_ptr> _tracks) :
			CDialogImpl<TaggerDialog>(),
			tracks(_tracks),
			loaded(false),
			current_release(0),
			current_disc(0)
		{
			mbc = new ReleaseList();
			Create(core_api::get_main_window());
			threaded_process::g_run_modeless(new service_impl_t<foo_musicbrainz::RequestThread>(query, m_hWnd, mbc),
				threaded_process::flag_show_progress | threaded_process::flag_show_abort, m_hWnd, "Quering information from MusicBrainz");
		}

		BEGIN_MSG_MAP(TaggerDialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			MSG_WM_CLOSE(OnClose)
			NOTIFY_HANDLER_EX(IDC_RELEASE_LIST, LVN_ITEMCHANGED, OnReleaseListChange)
			NOTIFY_HANDLER_EX(IDC_TRACK_LIST, NM_CLICK, OnTrackListClick)
			NOTIFY_HANDLER_EX(IDC_URL, NM_CLICK, OnLink)
			NOTIFY_HANDLER_EX(IDC_URL, NM_RETURN, OnLink)
			COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
			COMMAND_ID_HANDLER_EX(IDOK, OnOk)
			COMMAND_HANDLER_EX(IDC_TYPE, CBN_SELENDOK, OnTypeChange)
			COMMAND_HANDLER_EX(IDC_STATUS, CBN_SELENDOK, OnStatusChange)
			COMMAND_HANDLER_EX(IDC_ARTIST, EN_UPDATE, OnArtistUpdate)
			COMMAND_HANDLER_EX(IDC_ALBUM, EN_UPDATE, OnAlbumUpdate)
			COMMAND_HANDLER_EX(IDC_DATE, EN_UPDATE, OnDateUpdate)
			//CHAIN_MSG_MAP(CDialogImpl<TaggerDialog>)
		END_MSG_MAP()

		Release *get_current_release() {
			return mbc->get(current_release);
		}

		Medium *get_current_medium() {
			return get_current_release()->get_medium_list()->get(current_disc);
		}

		bool OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
			static_api_ptr_t<modeless_dialog_manager>()->add(m_hWnd);
			release_list = GetDlgItem(IDC_RELEASE_LIST);
			medium_list = GetDlgItem(IDC_DISC_LIST);
			track_list = GetDlgItem(IDC_TRACK_LIST);
			type = GetDlgItem(IDC_TYPE);
			status = GetDlgItem(IDC_STATUS);
			url = GetDlgItem(IDC_URL);
			artist = GetDlgItem(IDC_ARTIST);
			album = GetDlgItem(IDC_ALBUM);
			date = GetDlgItem(IDC_DATE);
			disc = GetDlgItem(IDC_DISC);
			discsubtitle = GetDlgItem(IDC_DISCSUBTITLE);
			track_list_view.Attach(track_list, mbc);
		
			// List view styles
			auto styles = LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP;
			release_list.SetExtendedListViewStyle(styles, styles);
			medium_list.SetExtendedListViewStyle(styles, styles);
			track_list.SetExtendedListViewStyle(styles, styles);

			// Adding release list columns
			listview_helper::insert_column(release_list, 0, "Artist", 115);
			listview_helper::insert_column(release_list, 1, "Release", 115);
			listview_helper::insert_column(release_list, 2, "Date", 49);

			// Disc list columns
			listview_helper::insert_column(medium_list, 0, "", 0); // Fake column
			medium_list.InsertColumn(1, L"#", LVCFMT_RIGHT, 30);
			medium_list.DeleteColumn(0);
			listview_helper::insert_column(medium_list, 1, "Title", 49);

			// Adding track list columns
			listview_helper::insert_column(track_list, 0, "", 0); // Fake column
			track_list.InsertColumn(1, L"#", LVCFMT_RIGHT, 30);
			track_list.DeleteColumn(0);
			listview_helper::insert_column(track_list, 1, "Title", 260);

			for (auto i = 0; i < ReleaseGroup::type_count; i++) {
				pfc::stringcvt::string_os_from_utf8 str(ReleaseGroup::types[i]);
				type.AddString(str);
			}
			for (auto i = 0; i < Release::status_count; i++) {
				pfc::stringcvt::string_os_from_utf8 str(Release::statuses[i]);
				status.AddString(str);
			}
			return true;
		}

		void OnShowWindow(BOOL bShow, UINT nStatus) {
			SetMsgHandled(FALSE);
			if (loaded) return;
			loaded = true;
			
			for (unsigned int i = 0; i < mbc->count(); i++) {
				listview_helper::insert_item(release_list, i, mbc->get(i)->get_artist_credit()->get_name(), NULL);
				listview_helper::set_item_text(release_list, i, 1, (*mbc)[i]->get_title());
				listview_helper::set_item_text(release_list, i, 2, static_cast<pfc::string8>((*mbc)[i]->get_date()).get_ptr());
			}

			Release *release = get_current_release();
			// char track_number_str[10];

			//for (unsigned int i = 0; i < release->track_list(); i++)
			//{
			//	sprintf(track_number_str, "%u", i+1);
			//	listview_helper::insert_item(track_list, i, track_number_str, NULL);
			//}
			UpdateRelease();
		}

		void UpdateRelease() {
			Release *release = get_current_release();

			uSetWindowText(artist, release->get_artist_credit()->get_name());
			uSetWindowText(album, release->get_title());
			uSetWindowText(date, static_cast<pfc::string8>(release->get_date()));

			type.SetCurSel(get_current_release()->get_release_group()->get_type_index());
			status.SetCurSel(get_current_release()->get_status_index());

			// VA?
			if (track_list.GetColumnWidth(2) && !release->is_various()) {
				track_list.SetColumnWidth(1, 260);
				track_list.DeleteColumn(2);
			} else if (!track_list.GetColumnWidth(2) && release->is_various()) {
				track_list.SetColumnWidth(1, 195);
				listview_helper::insert_column(track_list, 2, "Track artist", 130);
			}

			// Discs
			auto media = get_current_release()->get_medium_list();
			medium_list.Resize(media->count());
			for (size_t item = 0; item < media->count(); item++) {
				pfc::string8 position;
				position << media->get(item)->get_position();
				listview_helper::set_item_text(medium_list, item, 0, position);
				listview_helper::set_item_text(medium_list, item, 1, media->get(item)->get_title());
			}

			// Link
			pfc::string8 url_string = "<a href=\"http://musicbrainz.org/release/";
			url_string += release->get_id();
			url_string += "\">MusicBrainz release page</a>";
			uSetWindowText(url, url_string);
		}

		LRESULT OnTrackListClick(LPNMHDR pnmh) {
			if (((LPNMITEMACTIVATE)pnmh)->iItem != -1 && ((LPNMITEMACTIVATE)pnmh)->iSubItem != 0) {
				track_list_view.Start(((LPNMITEMACTIVATE)pnmh)->iItem, ((LPNMITEMACTIVATE)pnmh)->iSubItem);
			}
			return 0;
		}

		LRESULT OnReleaseListChange(LPNMHDR pnmh) {
			if (((LPNMLISTVIEW)pnmh)->iItem != -1 && ((LPNMLISTVIEW)pnmh)->uChanged & LVIS_DROPHILITED && ((LPNMLISTVIEW)pnmh)->uNewState & LVIS_SELECTED) {
				if (current_release != ((LPNMITEMACTIVATE)pnmh)->iItem) {
					if (track_list_view.IsActive()) track_list_view.Abort();
					current_release = ((LPNMITEMACTIVATE)pnmh)->iItem;
					UpdateRelease();
				}
			}
			return 0;
		}

		LRESULT OnLink(LPNMHDR pnmh) {
			ShellExecute(NULL, L"open", ((PNMLINK)pnmh)->item.szUrl, NULL, NULL, SW_SHOW);
			return 0;
		}

		void OnClose() {
			DestroyWindow();
		}

		void OnFinalMessage(HWND hwnd) {
			static_api_ptr_t<modeless_dialog_manager>()->remove(m_hWnd);
			if (mbc) delete mbc;
			delete this;
		}

		void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl) {
			DestroyWindow();
		}

		void OnOk(UINT uNotifyCode, int nID, CWindow wndCtl) {
			static_api_ptr_t<metadb_io_v2>()->update_info_async(tracks,
				new service_impl_t<TagWriter>(mbc, tracks, current_release),
				core_api::get_main_window(), metadb_io_v2::op_flag_delay_ui, nullptr);
			mbc = NULL;
			DestroyWindow();
		}

		void OnTypeChange(UINT uNotifyCode, int nID, CWindow wndCtl) {
			get_current_release()->get_release_group()->set_type(type.GetCurSel());
		}

		void OnStatusChange(UINT uNotifyCode, int nID, CWindow wndCtl) {
			get_current_release()->set_status(status.GetCurSel());
		}

		void OnArtistUpdate(UINT uNotifyCode, int nID, CWindow wndCtl) {
			pfc::string8 str;
			uGetWindowText(artist, str);
			get_current_release()->get_artist_credit()->set_name(str);
			listview_helper::set_item_text(release_list, current_release, 0, str);
		}

		void OnAlbumUpdate(UINT uNotifyCode, int nID, CWindow wndCtl) {
			pfc::string8 str;
			uGetWindowText(album, str);
			get_current_release()->set_title(str);
			listview_helper::set_item_text(release_list, current_release, 1, str);
		}

		void OnDateUpdate(UINT uNotifyCode, int nID, CWindow wndCtl) {
			pfc::string8 str;
			uGetWindowText(date, str);
			get_current_release()->set_date(Date(str));
			listview_helper::set_item_text(release_list, current_release, 2, str);
		}
	};
}
