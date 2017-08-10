/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef NET_EATHENA_MAIL2RECV_H
#define NET_EATHENA_MAIL2RECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace Mail2Recv
    {
        void processMailIcon(Net::MessageIn &msg);
        void processOpenNewMailWindow(Net::MessageIn &msg);
        void processAddItemResult(Net::MessageIn &msg);
        void processRemoveItemResult(Net::MessageIn &msg);
        void processCheckNameResult(Net::MessageIn &msg);
        void processSendResult(Net::MessageIn &msg);
        void processMailListPage(Net::MessageIn &msg);
    }  // namespace MailRecv
}  // namespace EAthena

#endif  // NET_EATHENA_MAIL2RECV_H
