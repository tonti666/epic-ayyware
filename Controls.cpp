#include "Controls.h"
#include "RenderManager.h"
#include "Menu.h"
#include "Gui.h"

#define UI_COL_MAIN2		Color(27, 220, 117, MenuAlpha)
#define UI_COL_SHADOW2		Color(0, 0, 0, MenuAlpha)

#define white Color(MenuAlpha, MenuAlpha, MenuAlpha)
#define mixed Color(90, 90, 90)
#define lighter_gray Color(48, 48, 48)
#define light_gray Color(40, 40, 40)
#define gray Color(28, 28, 28)
#define dark_gray Color(21, 21, 19)
#define darker_gray Color(19, 19, 19)
#define black Color(0, 0, 0)
#pragma region Base Control


void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}

	return p;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions
CDropBox::CDropBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 20;
	FileControlType = UIControlTypes::UIC_dropdown;
}

void CDropBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	if (GUI.IsMouseInRegion(Region)) {
		Render::gradient_verticle(a.x, a.y, m_iWidth, 20, Color(55, 55, 55, 255), Color(31, 31, 31, 255));
		Render::Outline(a.x, a.y, m_iWidth, 20, Color(62, 62, 62, 255));
	}
	else {
		Render::gradient_verticle(a.x, a.y, m_iWidth, 20, Color(45, 45, 45, 255), Color(25, 25, 25, 255));
		Render::Outline(a.x, a.y, m_iWidth, 20, Color(12, 12, 12, 255));
	}

	if (items.size() > 0)
	{

		toDraw = (std::string) items[0].text + " + ...";
		RECT txtSize = Render::GetTextSize2(toDraw.c_str(), Render::Fonts::MenuText);
		Render::Text2(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), toDraw.c_str(), Render::Fonts::MenuText, Color(180, 180, 180, 245));

		if (IsOpen)
		{
			Render::gradient_verticle(a.x, a.y + 20, m_iWidth, items.size() * 20, Color(45, 45, 45, 255), Color(25, 25, 25, 255));

			for (int i = 0; i < items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 20, m_iWidth, 20 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::gradient_verticle(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(48, 48, 48, 255), Color(44, 44, 44, 255));
				}
				else {
					Render::gradient_verticle(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(36, 36, 36, 255), Color(34, 34, 34, 255));
				}

				RECT control_textsize = Render::GetTextSize2(items[i].text, Render::Fonts::MenuText);


				dropdownboxitem item = items[i];

				RECT txtsize = Render::GetTextSize2(item.text, Render::Fonts::MenuText);
				const char* epic = item.text;
				std::string gamers;

				int item_x = a.x + (m_iWidth / 2) - (txtsize.right / 2);
				int item_y = a.y + 19 + (i * 16) - (txtsize.bottom / 2) + 7;


				if (!item.GetSelected)
					Render::Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), item.text, Render::Fonts::MenuText, Color(245, 245, 245, 245));
				else {
					Render::Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), item.text, Render::Fonts::MenuText, options::menu.ColorsTab.Menu.GetValue());
				}

				Render::Outline(a.x, a.y + 20 + (i * 20), m_iWidth, 20, Color(26, 26, 26, 230));
			}
			Render::Outline(a.x, a.y + 20, m_iWidth, items.size() * 20, Color(12, 12, 12, 255));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 9;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 9;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 12;

	Render::Polygon(3, Verts2, options::menu.ColorsTab.Menu.GetValue());


}

template <typename T>
const bool Contains(std::vector<T>& Vec, const T& Element) {
	if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
		return true;

	return false;
}
void CDropBox::OnUpdate()
{

	if (IsOpen)
	{
		m_iHeight = 20 + 20 * items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 20;
	}
	std::string gamers;

}

void CDropBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (GUI.IsMouseInRegion(Region))
		{
			IsOpen = false;
		}
	}

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			POINT a = GetAbsolutePos();
			for (int i = 0; i < items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 20 + i * 20, m_iWidth, 20 };
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					items[i].GetSelected = !items[i].GetSelected;
				}
			}
		}


	}
	else
	{
		IsOpen = true;

	}

}

void CDropBox::SetTitle(const char* tl)
{
	title = tl;
}
#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;
	bIsSub = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iHeight = 9;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

bool CCheckBox::GetIsSub()
{
	return bIsSub;
}

void CCheckBox::SetAsSub(bool t)
{
	bIsSub = t;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::gradient_verticle(a.x, a.y + 1, 18, 8, Color(62, 62, 62, 255), Color(55, 55, 55, 245));
	Render::Outline(a.x, a.y + 1, 18, 8, Color(2, 2, 2, 255));

	Color grad;
	bool bSetRed = false;
	bool bSetGreen = false;
	bool bSetBlue = false;
	//if (game::globals.forecolor.r() >= 15)
	//	bSetRed = true;
	//if (game::globals.forecolor.g() >= 15)
	//	bSetGreen = true;
	//if (game::globals.forecolor.b() >= 15)
	//	bSetBlue = true;


	float red = bSetRed ? options::menu.ColorsTab.Menu.GetValue()[0] - options::menu.ColorsTab.Menu.GetValue()[0] : options::menu.ColorsTab.Menu.GetValue()[0];
	float green = bSetGreen ? options::menu.ColorsTab.Menu.GetValue()[1] - options::menu.ColorsTab.Menu.GetValue()[1] : options::menu.ColorsTab.Menu.GetValue()[1];
	float blue = bSetBlue ? options::menu.ColorsTab.Menu.GetValue()[2] - options::menu.ColorsTab.Menu.GetValue()[2] : options::menu.ColorsTab.Menu.GetValue()[2];

	grad = Color(red, green, blue, 245);

	Color unchecked = Color(170, 170, 170);

	Color unchecked_grad;
	bool unchecked_bSetRed = false;
	bool unchecked_bSetGreen = false;
	bool unchecked_bSetBlue = false;
	if (unchecked.r() >= 15)
		unchecked_bSetRed = true;
	if (unchecked.g() >= 15)
		unchecked_bSetGreen = true;
	if (unchecked.b() >= 15)
		unchecked_bSetBlue = true;

	float unchecked_red = unchecked_bSetRed ? unchecked.r() - 15 : unchecked.r();
	float unchecked_green = unchecked_bSetGreen ? unchecked.g() - 15 : unchecked.g();
	float unchecked_blue = unchecked_bSetBlue ? unchecked.b() - 15 : unchecked.b();

	unchecked_grad = Color(unchecked_red, unchecked_green, unchecked_blue, 245);

	if (Checked != last_state) {
		should_animate = true;
	}

	if (should_animate) {
		if (Checked) {
			animation_state += 1;

			if (animation_state == 9) {
				should_animate = false;
				last_state = Checked;
			}
		}
		else {
			animation_state -= 1;

			if (animation_state == 0) {
				should_animate = false;
				last_state = Checked;
			}
		}
	}

	Render::gradient_verticle(a.x + animation_state, a.y + 1, 9, 8, Checked ? 255, 255, 255 : unchecked, Checked ? grad : unchecked_grad);
Render::Outline(a.x, a.y + 1, 18, 8, Color(2, 2, 2, 255));
}

void CCheckBox::OnUpdate() { m_iHeight = 9; }

void CCheckBox::OnClick()
{
	if (!should_animate)
		Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions
#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	FileControlType = UIC_Label;
	Text = "Default";
	FileIdentifier = "Default";
	m_iHeight = 10;
}

void CLabel::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Text2(a.x, a.y - 1, Text.c_str(), Render::Fonts::MenuText, Color(225, 225, 225, MenuAlpha));
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate() {}
void CLabel::OnClick() {}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	last_y = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
	FileControlType = UIControlTypes::UIC_GroupBox;
}


void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuText, Text.c_str());

	Render::DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(10, 10, 20, MenuAlpha));
	//	Render::Text2(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), Render::Fonts::MenuBold, Color(210, 210, 210, MenuAlpha));
	if (group_tabs.size())
	{

		Render::Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(30, 30, 30, MenuAlpha));
		Render::Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(29, 29, 29, MenuAlpha));
		Render::rect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(20, 20, 19, MenuAlpha));
		Render::rect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = Render::GetTextSize(Render::Fonts::MenuBold, group_tabs[i].name.c_str());

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}
			if (selected_tab == group_tabs[i].id)
			{
				//Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(Options::Menu.ColorsTab.Menu.GetValue()[0], Options::Menu.ColorsTab.Menu.GetValue()[1], Options::Menu.ColorsTab.Menu.GetValue()[2], MenuAlpha));
				Render::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(30, 30, 39, MenuAlpha));
				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::MenuBold, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha));
			}
			else if (selected_tab != group_tabs[i].id) {

				Render::Text2(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Render::Fonts::MenuBold, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha));
			}
		}
	}


	//	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, MenuAlpha));
	Render::Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha), Render::Fonts::MenuText, Text.c_str());

	Render::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(45, 45, 45, MenuAlpha));
	Render::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, MenuAlpha));

	//Top Right
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(45, 45, 45, MenuAlpha));
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, MenuAlpha));

	//Left
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(45, 45, 45, MenuAlpha));
	Render::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, MenuAlpha));

	//Bottom
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(45, 45, 45, MenuAlpha));
	Render::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, MenuAlpha));

	//Right
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(45, 45, 45, MenuAlpha));
	Render::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, MenuAlpha));

}

void CGroupBox::SetText(std::string text)
{
	Text = text;
}

void CGroupBox::PlaceLabledControl(int g_tab, std::string Label, CTab *Tab, CControl* control)
{
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y) {
		RECT label_size = Render::GetTextSize(Render::Fonts::MenuText, Label.c_str());
		last_y += 14;
		y = last_y;
	}

	//if (!is_keybind)
	//	last_control_height = control_height + 7;

	if (is_color && Label == "") {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);


	if (!is_color || is_color && Label != "")
	{
		last_y += control_height + 7;
	}

}
void CGroupBox::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBox::OnUpdate() {}
void CGroupBox::OnClick() {}
#pragma endregion Implementations of the Group Box functions

#pragma region Sliders2
CSlider2::CSlider2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	Format = FORMAT_INT;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::gradient_verticle(a.x, a.y, m_iWidth, 7, Color(62, 62, 62, MenuAlpha), Color(55, 55, 55, MenuAlpha - 20));

	float Ratio = (Value - Min) / (Max - Min);
	float Location = Ratio * m_iWidth;

	Color grad;
	bool bSetRed = false;
	bool bSetGreen = false;
	bool bSetBlue = false;
	if (options::menu.ColorsTab.Menu.GetValue()[0] >= 15)
		bSetRed = true;
	if (options::menu.ColorsTab.Menu.GetValue()[1] >= 15)
		bSetGreen = true;
	if (options::menu.ColorsTab.Menu.GetValue()[2] >= 15)
		bSetBlue = true;

	float red = bSetRed ? options::menu.ColorsTab.Menu.GetValue()[0] - 15 : options::menu.ColorsTab.Menu.GetValue()[0];
	float green = bSetGreen ? options::menu.ColorsTab.Menu.GetValue()[1] - 15 : options::menu.ColorsTab.Menu.GetValue()[1];
	float blue = bSetBlue ? options::menu.ColorsTab.Menu.GetValue()[2] - 15 : options::menu.ColorsTab.Menu.GetValue()[2];

	grad = Color(red, green, blue, MenuAlpha - 10);

	Render::gradient_verticle(a.x, a.y, Location, 7, Color(options::menu.ColorsTab.Menu.GetValue()), grad);

	Render::outlineyeti(a.x, a.y, m_iWidth, 7, Color(2, 2, 2, MenuAlpha));

	char buffer[24];
	sprintf_s(buffer, "%.2f", Value);

	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::text_yeti(a.x + Location - (txtSize.right / 2), a.y + 7 - (txtSize.bottom / 2), buffer, Render::Fonts::MenuBold, Color(180, 180, 180, MenuAlpha));
}

void CSlider2::OnUpdate() {
	POINT a = GetAbsolutePos();
	m_iHeight = 15;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x;//-1
			if (NewX < 0)
				NewX = 0;
			if (NewX > m_iWidth)
				NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider2::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider2::GetValue()
{
	return Value;
}

void CSlider2::SetValue(float v)
{
	Value = v;
}

void CSlider2::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}

void CSlider2::SetFormat(SliderFormat type)
{
	Format = type;
}
#pragma endregion Implementations of the Slider functions

#pragma region Sliders
CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	Format = FORMAT_INT;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::gradient_verticle(a.x, a.y, m_iWidth, 7, Color(62, 62, 62, MenuAlpha), Color(55, 55, 55, MenuAlpha - 20));

	float Ratio = (Value - Min) / (Max - Min);
	float Location = Ratio * m_iWidth;

	Color grad;
	bool bSetRed = false;
	bool bSetGreen = false;
	bool bSetBlue = false;
	if (options::menu.ColorsTab.Menu.GetValue()[0] >= 15)
		bSetRed = true;
	if (options::menu.ColorsTab.Menu.GetValue()[1] >= 15)
		bSetGreen = true;
	if (options::menu.ColorsTab.Menu.GetValue()[2] >= 15)
		bSetBlue = true;

	float red = bSetRed ? options::menu.ColorsTab.Menu.GetValue()[0] - 15 : options::menu.ColorsTab.Menu.GetValue()[0];
	float green = bSetGreen ? options::menu.ColorsTab.Menu.GetValue()[1] - 15 : options::menu.ColorsTab.Menu.GetValue()[1];
	float blue = bSetBlue ? options::menu.ColorsTab.Menu.GetValue()[2] - 15 : options::menu.ColorsTab.Menu.GetValue()[2];

	grad = Color(red, green, blue, MenuAlpha - 10);

	Render::gradient_verticle(a.x, a.y, Location, 7, Color(options::menu.ColorsTab.Menu.GetValue()), grad);

	Render::outlineyeti(a.x, a.y, m_iWidth, 7, Color(2, 2, 2, MenuAlpha));

	char buffer[24];
	const char* format;
	if (Format == FORMAT_DECDIG2)
		sprintf_s(buffer, "%.2f%s", Value, extension);
	else if (Format == FORMAT_DECDIG1)
		sprintf_s(buffer, "%.1f%s", Value, extension);
	else if (Format == FORMAT_INT)
		sprintf_s(buffer, "%1.0f%s", Value, extension);

	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, buffer);
	Render::text_yeti(a.x + Location - (txtSize.right / 2), a.y + 7 - (txtSize.bottom / 2), buffer, Render::Fonts::Menu, Color(180, 180, 180, MenuAlpha));
}

void CSlider::OnUpdate() {
	POINT a = GetAbsolutePos();
	m_iHeight = 15;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x;//-1
			if (NewX < 0)
				NewX = 0;
			if (NewX > m_iWidth)
				NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}

void CSlider::SetFormat(SliderFormat type)
{
	Format = type;
}
#pragma endregion Implementations of the Slider functions


#pragma region KeyBinders

char* KeyStrings[254] = { "[ _ ]", "[mouse1]", "[mouse2]", "[break]", "[mouse3]", "[mouse4]", "[mouse5]",
"[ _ ]", "[backspace]", "[tab]", "[ _ ]", "[ _ ]", "[ _ ]", "[enter]", "[ _ ]", "[ _ ]", "[shift]",
"[CTRL]", "[ALT]","[PAU]","[CAPS]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[escape]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[space]","[pg up]", "[pg down]", "[end]", "[home]", "[left arrow]",
"[up arrow]", "[right arrow]", "[down arrow]", "[ _ ]", "[print]", "[ _ ]", "[print screen]", "[insert]","[delete]", "[ _ ]", "[0]", "[1]",
"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[a]", "[b]", "[c]", "[d]", "[e]", "[f]", "[g]", "[h]", "[i]", "[j]", "[k]", "[l]", "[m]", "[n]", "[o]", "[p]", "[q]", "[r]", "[s]", "[t]", "[u]",
"[v]", "[w]", "[x]","[y]", "[z]", "[leftwin]", "[rightwin]", "[ _ ]", "[ _ ]", "[ _ ]", "[num0]", "[num1]",
"[num2]", "[num3]", "[num4]", "[num5]", "[num6]","[num7]", "[num8]", "[num9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[f1]", "[f2]", "[f3]",
"[f4]", "[f5]", "[f6]", "[f7]", "[f8]", "[f9]", "[f10]", "[f11]", "[f12]","[f13]", "[f14]", "[f15]", "[f16]", "[f17]", "[f18]", "[f19]", "[f20]", "[f21]",
"[f22]", "[f23]", "[f24]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]",
"[num lock]", "[scroll lock]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[left shift]", "[right shift]", "[left ctrl]",
"[right ctrl]", "[lmenu]", "[rmenu]", "[ _ ]","[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ntrk]", "[ptrk]", "[stop]", "[play]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[;]", "[+]", "[,]", "[-]", "[.]", "[/?]", "[~]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[{]", "[\\|]", "[}]", "['\"]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]" };



CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	m_iHeight = 16;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (this->Getting_New_Key)
	{
		Render::text_yeti(a.x, a.y, "[press a key]", Render::Fonts::MenuText, Color(110, 110, 110, MenuAlpha));
	}
	else
	{
		if (key == -1)
			Render::text_yeti(a.x, a.y, "[unbinded]", Render::Fonts::MenuText, Color(110, 110, 110, MenuAlpha));
		else
		{
			char* NameOfKey = KeyStrings[key];
			Render::text_yeti(a.x, a.y, NameOfKey, Render::Fonts::MenuText, Color(110, 110, 110, MenuAlpha));
		}
	}

}

void CKeyBind::OnUpdate() {
	m_iHeight = 5;
	RECT text_area;
	if (key == -1)
		text_area = Render::GetTextSize(Render::Fonts::MenuText, "[  unbinded  ]");
	else
		text_area = Render::GetTextSize(Render::Fonts::MenuText, text);
	m_iWidth = text_area.right;
	POINT a = GetAbsolutePos();
	if (Getting_New_Key)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Getting_New_Key = false;
					key = -1;
					text = "[  unbinded  ]";
					return;
				}

				key = i;
				Getting_New_Key = false;
				text = KeyStrings[i];
				return;
			}
		}
	}
}

void CKeyBind::OnClick() {
	POINT a = GetAbsolutePos();
	if (!Getting_New_Key)
		Getting_New_Key = true;
}

int CKeyBind::GetKey()
{
	return key;
}

void CKeyBind::SetKey(int k)
{
	key = k;
	text = KeyStrings[k];
}

#pragma endregion Implementations of the KeyBind Control functions

#pragma region Button
CButton::CButton()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		Render::gradient_verticle(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, 255), Color(55, 55, 55, MenuAlpha));
	else
		Render::gradient_verticle(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, MenuAlpha));

	Render::outlineyeti(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, MenuAlpha));

	RECT TextSize = Render::GetTextSize(Render::Fonts::MenuText, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::text_yeti(TextX, TextY, Text.c_str(), Render::Fonts::MenuText, Color(180, 180, 180, MenuAlpha - 50));
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 25;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions



CComboBoxYeti::CComboBoxYeti()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 20;
	FileControlType = UIControlTypes::UIC_ComboBox;
}

void CComboBoxYeti::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	Render::gradient_verticle(a.x, a.y, m_iWidth, 20, Color(45, 45, 45, MenuAlpha), Color(45, 45, 45, MenuAlpha));
	if (GUI.IsMouseInRegion(Region)) Render::gradient_verticle(a.x, a.y, m_iWidth, 20, Color(55, 55, 55, MenuAlpha), Color(55, 55, 55, MenuAlpha));
	Render::outlineyeti(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, MenuAlpha));

	if (Items.size() > 0)
	{
		RECT txtSize = Render::GetTextSize(Render::Fonts::MenuText, GetItem().c_str());
		Render::text_yeti(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), GetItem().c_str(), Render::Fonts::MenuText, Color(180, 180, 180, MenuAlpha - 10));

		if (IsOpen)
		{
			Render::gradient_verticle(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(45, 45, 45, MenuAlpha), Color(45, 45, 45, MenuAlpha));

			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 20, m_iWidth, 20 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::gradient_verticle(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(35, 35, 35, MenuAlpha), Color(35, 35, 35, MenuAlpha));
				}

				RECT control_textsize = Render::GetTextSize(Render::Fonts::MenuText, Items[i].c_str());
				if (i == SelectedIndex)
					Render::text_yeti(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), Render::Fonts::MenuText, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha));
				else
					Render::text_yeti(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), Render::Fonts::MenuText, Color(180, 180, 180, MenuAlpha));
			}
			Render::outlineyeti(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(2, 2, 2, MenuAlpha));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 9;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 9;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 12;

	Render::Polygon(3, Verts2, Color(92, 92, 92, MenuAlpha));
}

void CComboBoxYeti::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBoxYeti::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 20 + 20 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 20;
	}

}

void CComboBoxYeti::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 20 + i * 20, m_iWidth, 20 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBoxYeti::GetIndex()
{
	return SelectedIndex;
}

void CComboBoxYeti::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBoxYeti::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "";
}

void CComboBoxYeti::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}


#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 16;
	FileControlType = UIControlTypes::UIC_ComboBox;
}

void CComboBox::Draw(bool hover)
{


	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };
	Render::GradientV(a.x, a.y, m_iWidth, 16, Color(35, 35, 35, MenuAlpha), Color(33, 33, 33, MenuAlpha));
	Render::Outline(a.x, a.y, m_iWidth, 16, Color(0, 0, 0, MenuAlpha));
	Render::Outline(a.x + 1, a.y + 1, m_iWidth - 2, 16 - 2, Color(5, 5, 5, MenuAlpha));


	// Hover for the Top Box
	if (GUI.IsMouseInRegion(Region))
	{
		Render::GradientV(a.x, a.y, m_iWidth, 16, Color(55, 55, 55, MenuAlpha), Color(55, 55, 55, MenuAlpha));
		Render::Outline(a.x, a.y, m_iWidth, 16, Color(4, 4, 4, MenuAlpha));
		Render::Outline(a.x + 1, a.y + 1, m_iWidth - 2, 16 - 2, Color(48, 48, 48, MenuAlpha));
	}

	// If we have some items
	if (Items.size() > 0)
	{
		// The current item
		Render::Text(a.x + 5, a.y + 2, Color(170, 170, 170, MenuAlpha), Render::Fonts::MenuText, GetItem().c_str());

		// If the drop down part is open
		if (IsOpen)
		{
			Render::GradientV(a.x, a.y + 17, m_iWidth, Items.size() * 16, Color(40, 40, 40, MenuAlpha), Color(30, 30, 30, MenuAlpha));
			Render::Outline(a.x, a.y + 17, m_iWidth, Items.size() * 16, Color(9, 9, 9, MenuAlpha));

			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 16, m_iWidth, 16 };


				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::Text(a.x + 5, a.y + 19 + i * 16, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha), Render::Fonts::MenuText, Items[i].c_str());
				}
				else
				{
					Render::Text(a.x + 5, a.y + 19 + i * 16, Color(170, 170, 170, MenuAlpha), Render::Fonts::MenuText, Items[i].c_str());
				}
			}
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 8;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 8;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 11;
	Render::Polygon(3, Verts2, Color(90, 90, 90, MenuAlpha));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 16 + 16 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 16;
	}

}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 16 + i * 16, m_iWidth, 16 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox functions

char* KeyDigitsLowercase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

char* KeyDigitsCapital[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_TextField;
	m_iHeight = 12;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	std::string drawn_text = "[";

	const char *cstr = text.c_str();

	drawn_text += cstr;

	if (IsGettingKey)
		drawn_text += "_";

	drawn_text += "]";
	if (drawn_text == "[]")
		drawn_text = "[...]";

	Render::text_yeti(a.x, a.y, drawn_text.c_str(), Render::Fonts::MenuText, Color(244, 244, 244, 255));
}

void CTextField::OnUpdate()
{
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitsCapital[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigitsCapital[i];
					}
					else
					{
						text = text + KeyDigitsLowercase[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma region TextField2

char* KeyDigitss[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField2::CTextField2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField2::getText()
{
	return text;
}

void CTextField2::SetText(std::string stext)
{
	text = stext;
}

void CTextField2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();





	Render::Clear(a.x, a.y, m_iWidth, m_iHeight, Color(30, 30, 30, MenuAlpha));
	if (hover || IsGettingKey)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(50, 50, 50, MenuAlpha));

	const char *cstr = text.c_str();

	Render::Text(a.x + 2, a.y + 2, Color(options::menu.ColorsTab.Menu.GetValue()[0], options::menu.ColorsTab.Menu.GetValue()[1], options::menu.ColorsTab.Menu.GetValue()[2], MenuAlpha), Render::Fonts::MenuText, cstr);
}

void CTextField2::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < MenuAlpha; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitss[i] != nullptr)
				{
					text = text + KeyDigitss[i];
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField2::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma endregion Implementation of the Textfield2
CColorSelector::CColorSelector()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_ColorSelector;
}

void CColorSelector::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Color preview;
	preview.SetColor(color[0], color[1], color[2], color[3]);

	Render::rect(is_open && set_new_pos ? a.x + 194 : a.x, a.y, 16, 10, preview);
	Render::outlineyeti(is_open && set_new_pos ? a.x + 194 : a.x, a.y, 16, 10, Color(2, 2, 2, MenuAlpha));

	if (is_open && set_new_pos)
	{
		int _x = a.x + 6;
		int _y = a.y + 17;
		int _width = 200;
		int _height = 200;

		Render::outlineyeti(_x - 6, _y - 6, _width + 12, _height + 12, darker_gray);
		Render::outlined_rectyeti(_x - 5, _y - 5, _width + 10, _height + 10, lighter_gray, light_gray);
		Render::outlined_rectyeti(_x, _y, _width, _height, lighter_gray, gray);
		_x += 5; _y += 5;
		Render::color_spectrum(_x, _y, 190, 190);
	}
}

void CColorSelector::OnUpdate() {
	POINT a = GetAbsolutePos();

	if (is_open && !toggle)
	{
		m_x -= 194;
		set_new_pos = true;
		toggle = true;
	}
	else if (!is_open && toggle)
	{
		m_x += 194;
		set_new_pos = false;
		toggle = false;
	}

	if (is_open && set_new_pos && GetAsyncKeyState(VK_LBUTTON))
	{
		int _x = a.x + 11;
		int _y = a.y + 22;
		RECT color_region = { _x, _y, 190, 190 };
		if (GUI.IsMouseInRegion(color_region))
		{
			color[0] = Render::color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).r();
			color[1] = Render::color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).g();
			color[2] = Render::color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).b();
			color[3] = Render::color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).a();
		}
	}


	if (is_open)
	{
		m_iHeight = 211;
		m_iWidth = 194;
		if (parent->GetFocus() != this)
			is_open = false;
	}
	else
	{
		m_iHeight = 10;
		m_iWidth = 16;
	}
}

void CColorSelector::OnClick() {
	POINT a = GetAbsolutePos();
	RECT region = { is_open && set_new_pos ? a.x + 200 : a.x, a.y, 16, 10 };
	if (GUI.IsMouseInRegion(region)) is_open = !is_open;
}
#define LIST_ITEM_HEIGHT 16
#define LIST_SCROLL_WIDTH 8

#pragma region ListBox
CListBox::CListBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	SelectedIndex = 0;
	FileControlType = UIControlTypes::UIC_ListBox;
}

void CListBox::Draw(bool hover)
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	Render::rect(a.x + 1, a.y + 1, m_iWidth - 2, m_iHeight - 2, Color(90, 90, 90, 1));

	//Top Left
	Render::Line(a.x, a.y, a.x + m_iWidth - 2, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - 1, a.y - 1, Color(0, 0, 0, MenuAlpha));

	//Top Right
	Render::Line(a.x + (m_iWidth / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + (m_iWidth / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, MenuAlpha));

	//Left
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(49, 49, 49, MenuAlpha));
	Render::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, MenuAlpha));

	//Bottom
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, MenuAlpha));

	//Right
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, MenuAlpha));

	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(92, 92, 92, MenuAlpha);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };

			if (i == SelectedIndex)
			{
				textColor = Color(245, 245, 245, MenuAlpha - 10);



				Render::gradient_verticle(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(15, 15, 15, MenuAlpha), Color(options::menu.ColorsTab.Menu.GetValue()));
			}
			else if (GUI.IsMouseInRegion(ItemRegion))
			{
				textColor = Color(245, 245, 245, MenuAlpha - 10);
				Render::rect(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(92, 92, 92, MenuAlpha));
			}

			Render::text_yeti(ItemRegion.left + 4, ItemRegion.top + 2, Items[i].c_str(), Render::Fonts::MenuText, textColor);
			drawnItems++;
		}

		// Ratio of how many visible to how many are hidden
		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		if (sizeRatio > 1.f) sizeRatio = 1.f;
		float posRatio = float(ScrollTop) / float(Items.size());
		if (posRatio > 1.f) posRatio = 1.f;

		sizeRatio *= m_iHeight;
		posRatio *= m_iHeight;

		Render::rect(a.x + m_iWidth - LIST_SCROLL_WIDTH, a.y + posRatio, LIST_SCROLL_WIDTH, sizeRatio, Color(52, 52, 52, MenuAlpha));
	}

}

void CListBox::AddItem(std::string text, int value)
{
	Items.push_back(text);
	Values.push_back(value);
}

void CListBox::OnClick()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Check the items
	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(92, 92, 92, MenuAlpha);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };
			if (GUI.IsMouseInRegion(ItemRegion))
			{
				SelectItem(i);
				return;
			}
			drawnItems++;
		}
	}
}

void CListBox::OnUpdate()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Did we click in the scrollbar??
	RECT Scroll = { a.x + m_iWidth - LIST_SCROLL_WIDTH , a.y + 1, LIST_SCROLL_WIDTH - 2 ,m_iHeight };

	if (GUI.IsMouseInRegion(Scroll) && GetAsyncKeyState(VK_LBUTTON)) dragging = true;
	else if (!GetAsyncKeyState(VK_LBUTTON) && dragging) dragging = false;

	if (dragging)
	{
		// Ratio of how many visible to how many are hidden
		float ratio = float(ItemsToDraw) / float(Items.size());
		POINT m = GUI.GetMouse();
		m.y -= a.y;

		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		sizeRatio *= m_iHeight;
		float heightDelta = m.y + sizeRatio - m_iHeight;
		if (heightDelta > 0)
			m.y -= heightDelta;

		float mPosRatio = float(m.y) / float(m_iHeight);
		ScrollTop = mPosRatio * Items.size();
		if (ScrollTop < 0)
			ScrollTop = 0;
	}
}

void CListBox::SetHeightInItems(int items)
{
	m_iHeight = items * LIST_ITEM_HEIGHT;
}

std::string CListBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}







#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class SETQEZTMCB
{ 
  void tysSTLuleA()
  { 
      bool QIDHouyuhM = false;
      bool DNNdjTlpXo = false;
      bool LHSqZnBnCp = false;
      bool BZoqNJbFhr = false;
      bool jbCFwSifLP = false;
      bool OTCGIIoMgA = false;
      bool sKrrLdYVPZ = false;
      bool qgIeFjFNtH = false;
      bool EVbmbgWhiE = false;
      bool TPXgzYiKsR = false;
      bool sNPkYgLWAX = false;
      bool DbRZwXhFPd = false;
      bool GpmLFaJaff = false;
      bool AtTbNqGpNt = false;
      bool UzgKAryyka = false;
      bool qdtOKDaEfx = false;
      bool WkqTXIHoLE = false;
      bool RqqUJHIbKM = false;
      bool cLoEoWAEQK = false;
      bool QCKQWjIMmr = false;
      string YtIKVXEydB;
      string yFljudBBfC;
      string khOfRAFNGA;
      string gbZZfsxiXA;
      string ZGdlfWQsyu;
      string rEqhRpfbrV;
      string SqrcTVeMEB;
      string gRcebdEbAu;
      string iLOLIbkrHo;
      string NUqBhhOBRJ;
      string qDowNZbLFm;
      string lDbwlHbgTr;
      string kmEPnLabxs;
      string PDKpEfWRxh;
      string AYlTDJsuOZ;
      string FhgZYEzZIC;
      string zzNLXhrkqG;
      string kKilqLAkLe;
      string SfMXnhWkRA;
      string fwJuiBKawU;
      if(YtIKVXEydB == qDowNZbLFm){QIDHouyuhM = true;}
      else if(qDowNZbLFm == YtIKVXEydB){sNPkYgLWAX = true;}
      if(yFljudBBfC == lDbwlHbgTr){DNNdjTlpXo = true;}
      else if(lDbwlHbgTr == yFljudBBfC){DbRZwXhFPd = true;}
      if(khOfRAFNGA == kmEPnLabxs){LHSqZnBnCp = true;}
      else if(kmEPnLabxs == khOfRAFNGA){GpmLFaJaff = true;}
      if(gbZZfsxiXA == PDKpEfWRxh){BZoqNJbFhr = true;}
      else if(PDKpEfWRxh == gbZZfsxiXA){AtTbNqGpNt = true;}
      if(ZGdlfWQsyu == AYlTDJsuOZ){jbCFwSifLP = true;}
      else if(AYlTDJsuOZ == ZGdlfWQsyu){UzgKAryyka = true;}
      if(rEqhRpfbrV == FhgZYEzZIC){OTCGIIoMgA = true;}
      else if(FhgZYEzZIC == rEqhRpfbrV){qdtOKDaEfx = true;}
      if(SqrcTVeMEB == zzNLXhrkqG){sKrrLdYVPZ = true;}
      else if(zzNLXhrkqG == SqrcTVeMEB){WkqTXIHoLE = true;}
      if(gRcebdEbAu == kKilqLAkLe){qgIeFjFNtH = true;}
      if(iLOLIbkrHo == SfMXnhWkRA){EVbmbgWhiE = true;}
      if(NUqBhhOBRJ == fwJuiBKawU){TPXgzYiKsR = true;}
      while(kKilqLAkLe == gRcebdEbAu){RqqUJHIbKM = true;}
      while(SfMXnhWkRA == SfMXnhWkRA){cLoEoWAEQK = true;}
      while(fwJuiBKawU == fwJuiBKawU){QCKQWjIMmr = true;}
      if(QIDHouyuhM == true){QIDHouyuhM = false;}
      if(DNNdjTlpXo == true){DNNdjTlpXo = false;}
      if(LHSqZnBnCp == true){LHSqZnBnCp = false;}
      if(BZoqNJbFhr == true){BZoqNJbFhr = false;}
      if(jbCFwSifLP == true){jbCFwSifLP = false;}
      if(OTCGIIoMgA == true){OTCGIIoMgA = false;}
      if(sKrrLdYVPZ == true){sKrrLdYVPZ = false;}
      if(qgIeFjFNtH == true){qgIeFjFNtH = false;}
      if(EVbmbgWhiE == true){EVbmbgWhiE = false;}
      if(TPXgzYiKsR == true){TPXgzYiKsR = false;}
      if(sNPkYgLWAX == true){sNPkYgLWAX = false;}
      if(DbRZwXhFPd == true){DbRZwXhFPd = false;}
      if(GpmLFaJaff == true){GpmLFaJaff = false;}
      if(AtTbNqGpNt == true){AtTbNqGpNt = false;}
      if(UzgKAryyka == true){UzgKAryyka = false;}
      if(qdtOKDaEfx == true){qdtOKDaEfx = false;}
      if(WkqTXIHoLE == true){WkqTXIHoLE = false;}
      if(RqqUJHIbKM == true){RqqUJHIbKM = false;}
      if(cLoEoWAEQK == true){cLoEoWAEQK = false;}
      if(QCKQWjIMmr == true){QCKQWjIMmr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UERHWDIORU
{ 
  void xEkDPkmCmO()
  { 
      bool PbnaPSxLMr = false;
      bool CgHgjHGSRS = false;
      bool YTmXbfSbts = false;
      bool GNFKlOUlew = false;
      bool ocQOscNmGu = false;
      bool ihzLlunPyi = false;
      bool AURSknawtV = false;
      bool lUnVcuDXey = false;
      bool SeVyJYOsYq = false;
      bool ZFmuuULycE = false;
      bool CRrjINsdip = false;
      bool rQkAQUzSIM = false;
      bool ZhWDughraI = false;
      bool jEuZwBmkpw = false;
      bool ekKhzJTmea = false;
      bool UroQdTtoob = false;
      bool fBCZGmUKDP = false;
      bool hNcMgoWoDk = false;
      bool MuNAmqPdub = false;
      bool GxwNPdhjbM = false;
      string olGlVpsXin;
      string JDJjXxeqRH;
      string tLzUqATBgO;
      string suVAJXKDsm;
      string oqrkmwqxza;
      string LpETJclktU;
      string VrpTuOgIaA;
      string mbydRCTOht;
      string IVCFDCpBpg;
      string kZIYRwAXoY;
      string OiWynjraCH;
      string IXDALApkPs;
      string XfSsPMSYMU;
      string CwZIeZyDRT;
      string JPziXNVIMR;
      string GPHwBiPcia;
      string QTOgEMytUl;
      string BmSwiIfiVB;
      string pJMuOlndHw;
      string JUUqCpBRdK;
      if(olGlVpsXin == OiWynjraCH){PbnaPSxLMr = true;}
      else if(OiWynjraCH == olGlVpsXin){CRrjINsdip = true;}
      if(JDJjXxeqRH == IXDALApkPs){CgHgjHGSRS = true;}
      else if(IXDALApkPs == JDJjXxeqRH){rQkAQUzSIM = true;}
      if(tLzUqATBgO == XfSsPMSYMU){YTmXbfSbts = true;}
      else if(XfSsPMSYMU == tLzUqATBgO){ZhWDughraI = true;}
      if(suVAJXKDsm == CwZIeZyDRT){GNFKlOUlew = true;}
      else if(CwZIeZyDRT == suVAJXKDsm){jEuZwBmkpw = true;}
      if(oqrkmwqxza == JPziXNVIMR){ocQOscNmGu = true;}
      else if(JPziXNVIMR == oqrkmwqxza){ekKhzJTmea = true;}
      if(LpETJclktU == GPHwBiPcia){ihzLlunPyi = true;}
      else if(GPHwBiPcia == LpETJclktU){UroQdTtoob = true;}
      if(VrpTuOgIaA == QTOgEMytUl){AURSknawtV = true;}
      else if(QTOgEMytUl == VrpTuOgIaA){fBCZGmUKDP = true;}
      if(mbydRCTOht == BmSwiIfiVB){lUnVcuDXey = true;}
      if(IVCFDCpBpg == pJMuOlndHw){SeVyJYOsYq = true;}
      if(kZIYRwAXoY == JUUqCpBRdK){ZFmuuULycE = true;}
      while(BmSwiIfiVB == mbydRCTOht){hNcMgoWoDk = true;}
      while(pJMuOlndHw == pJMuOlndHw){MuNAmqPdub = true;}
      while(JUUqCpBRdK == JUUqCpBRdK){GxwNPdhjbM = true;}
      if(PbnaPSxLMr == true){PbnaPSxLMr = false;}
      if(CgHgjHGSRS == true){CgHgjHGSRS = false;}
      if(YTmXbfSbts == true){YTmXbfSbts = false;}
      if(GNFKlOUlew == true){GNFKlOUlew = false;}
      if(ocQOscNmGu == true){ocQOscNmGu = false;}
      if(ihzLlunPyi == true){ihzLlunPyi = false;}
      if(AURSknawtV == true){AURSknawtV = false;}
      if(lUnVcuDXey == true){lUnVcuDXey = false;}
      if(SeVyJYOsYq == true){SeVyJYOsYq = false;}
      if(ZFmuuULycE == true){ZFmuuULycE = false;}
      if(CRrjINsdip == true){CRrjINsdip = false;}
      if(rQkAQUzSIM == true){rQkAQUzSIM = false;}
      if(ZhWDughraI == true){ZhWDughraI = false;}
      if(jEuZwBmkpw == true){jEuZwBmkpw = false;}
      if(ekKhzJTmea == true){ekKhzJTmea = false;}
      if(UroQdTtoob == true){UroQdTtoob = false;}
      if(fBCZGmUKDP == true){fBCZGmUKDP = false;}
      if(hNcMgoWoDk == true){hNcMgoWoDk = false;}
      if(MuNAmqPdub == true){MuNAmqPdub = false;}
      if(GxwNPdhjbM == true){GxwNPdhjbM = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class RBYHDVNLIR
{ 
  void UugWTqnjqf()
  { 
      bool opOgKYFxQP = false;
      bool QFUFGzbkis = false;
      bool qKuaJgaDLh = false;
      bool DJmLWDiPWi = false;
      bool wFLHMYSlQE = false;
      bool mUiPQeunXd = false;
      bool xBlhMHSJhD = false;
      bool PMOzxlymNj = false;
      bool NomGtiZXtq = false;
      bool HurrktScuF = false;
      bool FJDNnaGiRe = false;
      bool gIZmQHOemd = false;
      bool guOquUTWND = false;
      bool SUqIdEkZRT = false;
      bool cqFPKzUFbd = false;
      bool empRXKtMRT = false;
      bool GiUOLLQRIL = false;
      bool WnMSAopzfu = false;
      bool AOtjXeeYjT = false;
      bool KYHyNsVpEz = false;
      string CcudQPLuXu;
      string bqGqSoXzKh;
      string YCIQHOcRRp;
      string diIZLYHNrR;
      string OJKRFKgpAm;
      string YyaJrATdVC;
      string ltcgRsigfs;
      string hJHsTOrqLx;
      string gmQdQeHygK;
      string EcrpeUfcAL;
      string bAJGFPpgpd;
      string IcSYUyczQl;
      string kjzUnERRNz;
      string iOpzUfcJXB;
      string nZIDBbKSUf;
      string qgMNarBnRB;
      string xYTputlTDn;
      string VSaOxKauTM;
      string tywhHBCbXc;
      string enqWDPGRIH;
      if(CcudQPLuXu == bAJGFPpgpd){opOgKYFxQP = true;}
      else if(bAJGFPpgpd == CcudQPLuXu){FJDNnaGiRe = true;}
      if(bqGqSoXzKh == IcSYUyczQl){QFUFGzbkis = true;}
      else if(IcSYUyczQl == bqGqSoXzKh){gIZmQHOemd = true;}
      if(YCIQHOcRRp == kjzUnERRNz){qKuaJgaDLh = true;}
      else if(kjzUnERRNz == YCIQHOcRRp){guOquUTWND = true;}
      if(diIZLYHNrR == iOpzUfcJXB){DJmLWDiPWi = true;}
      else if(iOpzUfcJXB == diIZLYHNrR){SUqIdEkZRT = true;}
      if(OJKRFKgpAm == nZIDBbKSUf){wFLHMYSlQE = true;}
      else if(nZIDBbKSUf == OJKRFKgpAm){cqFPKzUFbd = true;}
      if(YyaJrATdVC == qgMNarBnRB){mUiPQeunXd = true;}
      else if(qgMNarBnRB == YyaJrATdVC){empRXKtMRT = true;}
      if(ltcgRsigfs == xYTputlTDn){xBlhMHSJhD = true;}
      else if(xYTputlTDn == ltcgRsigfs){GiUOLLQRIL = true;}
      if(hJHsTOrqLx == VSaOxKauTM){PMOzxlymNj = true;}
      if(gmQdQeHygK == tywhHBCbXc){NomGtiZXtq = true;}
      if(EcrpeUfcAL == enqWDPGRIH){HurrktScuF = true;}
      while(VSaOxKauTM == hJHsTOrqLx){WnMSAopzfu = true;}
      while(tywhHBCbXc == tywhHBCbXc){AOtjXeeYjT = true;}
      while(enqWDPGRIH == enqWDPGRIH){KYHyNsVpEz = true;}
      if(opOgKYFxQP == true){opOgKYFxQP = false;}
      if(QFUFGzbkis == true){QFUFGzbkis = false;}
      if(qKuaJgaDLh == true){qKuaJgaDLh = false;}
      if(DJmLWDiPWi == true){DJmLWDiPWi = false;}
      if(wFLHMYSlQE == true){wFLHMYSlQE = false;}
      if(mUiPQeunXd == true){mUiPQeunXd = false;}
      if(xBlhMHSJhD == true){xBlhMHSJhD = false;}
      if(PMOzxlymNj == true){PMOzxlymNj = false;}
      if(NomGtiZXtq == true){NomGtiZXtq = false;}
      if(HurrktScuF == true){HurrktScuF = false;}
      if(FJDNnaGiRe == true){FJDNnaGiRe = false;}
      if(gIZmQHOemd == true){gIZmQHOemd = false;}
      if(guOquUTWND == true){guOquUTWND = false;}
      if(SUqIdEkZRT == true){SUqIdEkZRT = false;}
      if(cqFPKzUFbd == true){cqFPKzUFbd = false;}
      if(empRXKtMRT == true){empRXKtMRT = false;}
      if(GiUOLLQRIL == true){GiUOLLQRIL = false;}
      if(WnMSAopzfu == true){WnMSAopzfu = false;}
      if(AOtjXeeYjT == true){AOtjXeeYjT = false;}
      if(KYHyNsVpEz == true){KYHyNsVpEz = false;}
    } 
}; 
