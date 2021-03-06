/*
* Crossfire Debugger Support
*
* Copyright 2009 Niko Sams <niko.sams@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
#ifndef CROSSFIRELAUNCHCONFIG
#define CROSSFIRELAUNCHCONFIG

#include <interfaces/ilauncher.h>

namespace KDevelop
{
class ILaunchConfiguration;
}

namespace Crossfire
{
class CrossfirePlugin;

class BrowserLauncher : public KDevelop::ILauncher
{
public:
    BrowserLauncher( CrossfirePlugin* plugin );
    virtual QList< KDevelop::LaunchConfigurationPageFactory* > configPages() const;
    virtual QString description() const;
    virtual QString id();
    virtual QString name() const;
    virtual KJob* start(const QString& launchMode, KDevelop::ILaunchConfiguration* cfg);
    virtual QStringList supportedModes() const;

private:
    CrossfirePlugin* m_plugin;
    QList<KDevelop::LaunchConfigurationPageFactory*> m_factoryList;
};

}

#endif
