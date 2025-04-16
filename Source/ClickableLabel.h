/*
  ==============================================================================

    ClickableLabel.h
    Created: 15 Apr 2012 1:59:47pm
    Author:  Steve Folta

  ==============================================================================
*/

#ifndef __CLICKABLELABEL_H_F4AC0009__
#define __CLICKABLELABEL_H_F4AC0009__

#include <JuceHeader.h>

class ClickableLabel : public juce::Label {
	public:
		ClickableLabel(
			const juce::String& componentName = {},
			const juce::String& labelText = {});

		class JUCE_API ClickListener {
			public:
				virtual ~ClickListener() {}

				virtual void	labelClicked(Label* clickedLabel) = 0;
			};

		void	addClickListener(ClickListener* listener);
		void	removeClickListener(ClickListener* listener);

	protected:
		juce::ListenerList<ClickListener> clickListeners;

		void	mouseUp(const juce::MouseEvent& e) override;
	};




#endif  // __CLICKABLELABEL_H_F4AC0009__
