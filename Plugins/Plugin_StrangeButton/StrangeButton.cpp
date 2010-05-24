/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "StrangeButton.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_ISubWidgetText.h"

namespace plugin
{

	StrangeButton::StrangeButton() :
		mIsMousePressed(false),
		mIsMouseFocus(false),
		mIsStateCheck(false)
	{
	}

	void StrangeButton::initialiseWidgetSkin(MyGUI::ResourceSkin* _info)
	{
		Base::initialiseWidgetSkin(_info);

		// ������ ��������
		const MyGUI::MapString& properties = _info->getProperties();
		if (!properties.empty())
		{
			MyGUI::MapString::const_iterator iter = properties.find("ButtonPressed");
			if (iter != properties.end()) setButtonPressed(MyGUI::utility::parseBool(iter->second));
			iter = properties.find("StateCheck");
			if (iter != properties.end()) setStateCheck(MyGUI::utility::parseBool(iter->second));
		}
	}

	void StrangeButton::shutdownWidgetSkin()
	{
		Base::shutdownWidgetSkin();
	}

	void StrangeButton::onMouseSetFocus(MyGUI::Widget* _old)
	{
		Base::onMouseSetFocus(_old);
		mIsMouseFocus = true;

		if (mText == nullptr) return;
		std::string s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];

		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseLostFocus(MyGUI::Widget* _new)
	{
		Base::onMouseLostFocus(_new);
		mIsMouseFocus = false;

		if (mText == nullptr) return;
		std::string s, str = mText->getCaption();
		for (int i = (int)str.length() - 1; i >= 0; i--)
			s += str[i];
		
		mText->setCaption(s);

		updateButtonState();
	}

	void StrangeButton::onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mIsMousePressed = true;
			updateButtonState();
		}

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void StrangeButton::onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mIsMousePressed = false;
			updateButtonState();
		}

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void StrangeButton::updateButtonState()
	{
		if (mIsStateCheck)
		{
			if (!isEnabled()) { if (!setState("disabled_checked")) setState("disabled"); }
			else if (mIsMousePressed) { if (!setState("pushed_checked")) setState("pushed"); }
			else if (mIsMouseFocus) { if (!setState("highlighted_checked")) setState("pushed"); }
			else setState("normal_checked");
		}
		else
		{
			if (!isEnabled()) setState("disabled");
			else if (mIsMousePressed) setState("pushed");
			else if (mIsMouseFocus) setState("highlighted");
			else setState("normal");
		}
	}

	void StrangeButton::setStateCheck(bool _check)
	{
		if (mIsStateCheck == _check) return;
		mIsStateCheck = _check;
		updateButtonState();
	}

	void StrangeButton::_setMouseFocus(bool _focus)
	{
		mIsMouseFocus = _focus;
		updateButtonState();
	}

	void StrangeButton::baseUpdateEnable()
	{
		updateButtonState();
		if (!isEnabled())
		{
			mIsMouseFocus = false;
		}
	}

} // namespace plugin