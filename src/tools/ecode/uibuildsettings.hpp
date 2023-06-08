#ifndef EE_UI_UIBUILDSETTINGS_HPP
#define EE_UI_UIBUILDSETTINGS_HPP

#include "projectbuild.hpp"
#include <eepp/ui/uidatabind.hpp>
#include <eepp/ui/uirelativelayout.hpp>

using namespace EE::UI;

namespace ecode {

class UIBuildSettings : public UIRelativeLayout {
  public:
	static UIBuildSettings* New( ProjectBuild& build, ProjectBuildConfiguration& config,
								 bool isNew );

	virtual ~UIBuildSettings();

	void setTab( UITab* tab );

	UITab* getTab() const;

  protected:
	friend class UIBuildStep;

	ProjectBuild& mBuild;
	ProjectBuildConfiguration& mConfig;
	UIDataBindHolder mDataBindHolder;
	UITab* mTab{ nullptr };
	String mOldName;
	std::unordered_map<UIWidget*, std::vector<Uint32>> mCbs;
	ProjectBuildOutputParserConfig mTmpOpCfg;
	bool mIsNew{ false };
	bool mCanceled{ false };

	explicit UIBuildSettings( ProjectBuild& build, ProjectBuildConfiguration& config, bool isNew );

	void moveStepUp( size_t stepNum, bool isClea );

	void moveStepDown( size_t stepNum, bool isClean );

	void moveStepDir( size_t stepNum, bool isClean, int dir );

	void deleteStep( size_t stepNum, bool isClean );

	void updateOS();

	void refreshTab();

	void bindTable( const std::string& name, const std::string& key, ProjectBuildKeyVal& data );
};

} // namespace ecode

#endif // EE_UI_UIBUILDSETTINGS_HPP