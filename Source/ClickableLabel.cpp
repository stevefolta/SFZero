/*
  ==============================================================================

    ClickableLabel.cpp
    Created: 15 Apr 2012 1:59:47pm
    Author:  Steve Folta

  ==============================================================================
*/

#include "ClickableLabel.h"


ClickableLabel::ClickableLabel(
	const String& componentName,
	const String& labelText)
	: Label(componentName, labelText)
{
}


void ClickableLabel::addClickListener(ClickListener* listener)
{
	clickListeners.add(listener);
}


void ClickableLabel::removeClickListener(ClickListener* listener)
{
	clickListeners.remove(listener);
}


void ClickableLabel::mouseUp(const MouseEvent& e)
{
	bool goodClick =
		e.mouseWasClicked() && contains(e.getPosition()) && !e.mods.isPopupMenu();
	if (goodClick) {
		Component::BailOutChecker checker(this);
		clickListeners.callChecked(checker, &ClickListener::labelClicked, this);
		}
}



