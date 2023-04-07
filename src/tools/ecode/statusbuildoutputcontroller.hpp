#ifndef ECODE_STATUSBUILDOUTPUTCONTROLLER_HPP
#define ECODE_STATUSBUILDOUTPUTCONTROLLER_HPP

#include <eepp/ui/tools/uicodeeditorsplitter.hpp>
#include <eepp/ui/uicodeeditor.hpp>
#include <eepp/ui/uiscenenode.hpp>
#include <eepp/ui/uisplitter.hpp>

using namespace EE;
using namespace EE::UI;
using namespace EE::UI::Tools;

namespace ecode {

class App;

class StatusBuildOutputController {
  public:
	StatusBuildOutputController( UISplitter* mainSplitter, UISceneNode* uiSceneNode, App* app );

	void toggle();

	void hide();

	void show();

  protected:
	UISplitter* mMainSplitter{ nullptr };
	UISceneNode* mUISceneNode{ nullptr };
	App* mApp{ nullptr };
	UICodeEditorSplitter* mSplitter{ nullptr };
	UICodeEditor* mContainer{ nullptr };

	UICodeEditor* createContainer();
};

} // namespace ecode

#endif // ECODE_STATUSBUILDOUTPUTCONTROLLER_HPP
