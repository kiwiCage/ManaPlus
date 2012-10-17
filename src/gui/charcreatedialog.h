/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHAR_CREATE_DIALOG_H
#define CHAR_CREATE_DIALOG_H

#include "being.h"

#include "gui/charselectdialog.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

class Label;
class LocalPlayer;
class PlayerBox;
class RadioButton;
class TextField;

namespace gcn
{
    class Slider;
}

/**
 * Character creation dialog.
 *
 * \ingroup Interface
 */
class CharCreateDialog final : public Window,
                               public gcn::ActionListener,
                               public gcn::KeyListener
{
    public:
        /**
         * Constructor.
         */
        CharCreateDialog(CharSelectDialog *const parent, const int slot);

        A_DELETE_COPY(CharCreateDialog)

        /**
         * Destructor.
         */
        ~CharCreateDialog();

        void action(const gcn::ActionEvent &event) override;

        /**
         * Unlocks the dialog, enabling the create character button again.
         */
        void unlock();

        void setAttributes(const StringVect &labels,
                           const int available,
                           const int min, const int max);

        void setFixedGender(const bool fixed,
                            const Gender gender = GENDER_FEMALE);

        void logic() override;

        void updatePlayer();

        void keyPressed(gcn::KeyEvent &keyEvent) override;

    private:
        int getDistributedPoints() const;

        void updateSliders();

        /**
         * Returns the name of the character to create.
         */
        std::string getName() const;

        /**
         * Communicate character creation to the server.
         */
        void attemptCharCreate();

        void updateHair();

        void updateRace();

        CharSelectDialog *mCharSelectDialog;

        TextField *mNameField;
        Label *mNameLabel;
        Button *mNextHairColorButton;
        Button *mPrevHairColorButton;
        Label *mHairColorLabel;
        Label *mHairColorNameLabel;
        Button *mNextHairStyleButton;
        Button *mPrevHairStyleButton;
        Label *mHairStyleLabel;
        Label *mHairStyleNameLabel;
        Button *mNextRaceButton;
        Button *mPrevRaceButton;
        Label *mRaceLabel;
        Label *mRaceNameLabel;

        Button *mActionButton;
        Button *mRotateButton;

        RadioButton *mMale;
        RadioButton *mFemale;
        RadioButton *mOther;

        std::vector<gcn::Slider*> mAttributeSlider;
        std::vector<Label*> mAttributeLabel;
        std::vector<Label*> mAttributeValue;
        Label *mAttributesLeft;

        int mMaxPoints;
        int mUsedPoints;

        Button *mCreateButton;
        Button *mCancelButton;

        int mRace;

        Being *mPlayer;
        PlayerBox *mPlayerBox;

        int mHairStyle;
        int mHairColor;

        int mSlot;

        unsigned maxHairColor;
        unsigned minHairColor;
        unsigned maxHairStyle;
        unsigned minHairStyle;

        unsigned mAction;
        unsigned mDirection;
};

#endif // CHAR_CREATE_DIALOG_H
