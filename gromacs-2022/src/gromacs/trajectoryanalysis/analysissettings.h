/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright 2010- The GROMACS Authors
 * and the project initiators Erik Lindahl, Berk Hess and David van der Spoel.
 * Consult the AUTHORS/COPYING files and https://www.gromacs.org for details.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * https://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at https://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out https://www.gromacs.org.
 */
/*! \file
 * \brief
 * Declares gmx::TrajectoryAnalysisSettings.
 *
 * \author Teemu Murtola <teemu.murtola@gmail.com>
 * \inpublicapi
 * \ingroup module_trajectoryanalysis
 */
#ifndef GMX_TRAJECTORYANALYSIS_ANALYSISSETTINGS_H
#define GMX_TRAJECTORYANALYSIS_ANALYSISSETTINGS_H

#include <memory>
#include <string>

#include "gromacs/options/timeunitmanager.h"

namespace gmx
{

template<typename T>
class ArrayRef;

class AnalysisDataPlotSettings;
class ICommandLineOptionsModuleSettings;
class Options;
class TrajectoryAnalysisRunnerCommon;

/*! \brief
 * Trajectory analysis module configuration object.
 *
 * This class is used by trajectory analysis modules to inform the caller
 * about the requirements they have on the input (e.g., whether a topology is
 * required, or whether PBC removal makes sense).  It is also used to pass
 * similar information back to the analysis module after parsing user input.
 *
 * Having this functionality as a separate class makes the
 * TrajectoryAnalysisModule interface much cleaner, and also reduces the need to
 * change existing code when new options are added.
 *
 * Methods in this class do not throw, except for the constructor, which may
 * throw an std::bad_alloc.
 *
 * \todo
 * Remove plain flags from the public interface.
 *
 * \inpublicapi
 * \ingroup module_trajectoryanalysis
 */
class TrajectoryAnalysisSettings
{
public:
    //! Recognized flags.
    enum
    {
        /*! \brief
         * Forces loading of a topology file.
         *
         * If this flag is not specified, the topology file is loaded only
         * if it is provided on the command line explicitly.
         */
        efRequireTop = 1 << 0,
        /*! \brief
         * Requests topology coordinates.
         *
         * If this flag is specified, the position coordinates loaded from the
         * topology can be accessed, otherwise they are not loaded.
         *
         * \see TopologyInformation
         */
        efUseTopX = 1 << 1,
        /*! \brief
         * Requests topology coordinates.
         *
         * If this flag is specified, the velocity coordinates loaded from the
         * topology can be accessed, otherwise they are not loaded.
         *
         * \see TopologyInformation
         */
        efUseTopV = 1 << 2,
        /*! \brief
         * Disallows the user from changing PBC handling.
         *
         * If this option is not specified, the analysis module (see
         * TrajectoryAnalysisModule::analyzeFrame()) may be passed a NULL
         * PBC structure, and it should be able to handle such a situation.
         *
         * \see setPBC()
         */
        efNoUserPBC = 1 << 4,
        /*! \brief
         * Disallows the user from changing PBC removal.
         *
         * \see setRmPBC()
         */
        efNoUserRmPBC = 1 << 5,
    };

    //! Initializes default settings.
    TrajectoryAnalysisSettings();
    ~TrajectoryAnalysisSettings();

    //! Injects command line options module settings for some methods to use.
    void setOptionsModuleSettings(ICommandLineOptionsModuleSettings* settings);

    //! Returns the time unit the user has requested.
    TimeUnit timeUnit() const;
    //! Returns common settings for analysis data plot modules.
    const AnalysisDataPlotSettings& plotSettings() const;

    //! Returns the currently set flags.
    unsigned long flags() const;
    //! Tests whether a flag has been set.
    bool hasFlag(unsigned long flag) const;
    /*! \brief
     * Returns whether PBC should be used.
     *
     * Returns the value set with setPBC() and/or overridden by the user.
     * The user-provided value can be accessed in
     * TrajectoryAnalysisModule::optionsFinished(), and can be overridden
     * with a call to setPBC().
     */
    bool hasPBC() const;
    /*! \brief
     * Returns whether molecules should be made whole.
     *
     * See hasPBC() for information on accessing or overriding the
     * user-provided value.
     */
    bool hasRmPBC() const;
    //! Returns the currently set frame flags.
    int frflags() const;

    /*! \brief
     * Sets flags.
     *
     * Overrides any earlier set flags.
     * By default, no flags are set.
     */
    void setFlags(unsigned long flags);
    //! Sets or clears an individual flag.
    void setFlag(unsigned long flag, bool bSet = true);
    /*! \brief
     * Sets whether PBC are used.
     *
     * \param[in]  bPBC   true if PBC should be used.
     *
     * If called in TrajectoryAnalysisModule::initOptions(), this function
     * sets the default for whether PBC are used in the analysis.
     * If \ref efNoUserPBC is not set, a command-line option is provided
     * for the user to override the default value.
     * If called later, it overrides the setting provided by the user or an
     * earlier call.
     *
     * If this function is not called, the default is to use PBC.
     *
     * If PBC are not used, the \p pbc pointer passed to
     * TrajectoryAnalysisModule::analyzeFrame() is NULL.
     * The value of the flag can also be accessed with hasPBC().
     *
     * \see efNoUserPBC
     */
    void setPBC(bool bPBC);
    /*! \brief
     * Sets whether molecules are made whole.
     *
     * \param[in]     bRmPBC true if molecules should be made whole.
     *
     * If called in TrajectoryAnalysisModule::initOptions(), this function
     * sets the default for whether molecules are made whole.
     * If \ref efNoUserRmPBC is not set, a command-line option is provided
     * for the user to override the default value.
     * If called later, it overrides the setting provided by the user or an
     * earlier call.
     *
     * If this function is not called, the default is to make molecules
     * whole.
     *
     * The main use of this function is to call it with \c false if your
     * analysis program does not require whole molecules as this can
     * increase the performance.
     * In such a case, you can also specify \ref efNoUserRmPBC to not to
     * confuse the user with an option that would only slow the program
     * down.
     *
     * \see efNoUserRmPBC
     */
    void setRmPBC(bool bRmPBC);
    /*! \brief
     * Sets flags that determine what to read from the trajectory.
     *
     * \param[in]     frflags Flags for what to read from the trajectory file.
     *
     * If this function is not called, the flags default to TRX_NEED_X.
     * If the analysis module needs some other information (velocities,
     * forces), it can call this function to load additional information
     * from the trajectory.
     */
    void setFrameFlags(int frflags);

    //! \copydoc ICommandLineOptionsModuleSettings::setHelpText()
    void setHelpText(const ArrayRef<const char* const>& help);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;

    friend class TrajectoryAnalysisRunnerCommon;
};

} // namespace gmx

#endif
