/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#pragma once

#include "textchain.hxx"
#include <memory>

class SdrTextObj;
class SdrOutliner;
class NonOverflowingText;
class OverflowingText;
class TextChain;
class OutlinerParaObject;
class OFlowChainedText;
class UFlowChainedText;

class TextChainFlow
{
public:
    TextChainFlow(SdrTextObj* pChainTarget);
    virtual ~TextChainFlow();

    // Check for flow events in Outliner
    virtual void CheckForFlowEvents(SdrOutliner*);

    void ExecuteUnderflow(SdrOutliner*);

    // Uses two outliners: one for the non-overflow text and one for overflowing (might be the same)
    void ExecuteOverflow(SdrOutliner*, SdrOutliner*);

    bool IsOverflow() const;
    bool IsUnderflow() const;

    SdrTextObj* GetLinkTarget() const;

protected:
    // Cursor related
    bool mbPossiblyCursorOut;
    ESelection maOverflowPosSel;
    ESelection maPostChainingSel;

    std::unique_ptr<OFlowChainedText> mpOverflChText;
    std::unique_ptr<UFlowChainedText> mpUnderflChText;

    void impCheckForFlowEvents(SdrOutliner*, SdrOutliner*);

    TextChain* GetTextChain() const;

    virtual void impLeaveOnlyNonOverflowingText(SdrOutliner*);
    void impMoveChainedTextToNextLink(SdrOutliner*);

    virtual void impSetFlowOutlinerParams(SdrOutliner*, SdrOutliner*);

private:
    bool mbOFisUFinduced;

    void impUpdateCursorInfo();

    SdrTextObj* mpTargetLink;
    SdrTextObj* mpNextLink;

    TextChain* mpTextChain;

    bool bUnderflow;
    bool bOverflow;
};

// NOTE: EditingTextChainFlow might be strongly coupled with behavior in SdrTextObj::onChainingEvent
class EditingTextChainFlow : public TextChainFlow
{
public:
    EditingTextChainFlow(SdrTextObj*);
    virtual void CheckForFlowEvents(SdrOutliner*) override;

    //virtual void ExecuteOverflow(SdrOutliner *, SdrOutliner *) override;

protected:
    virtual void impLeaveOnlyNonOverflowingText(SdrOutliner*) override;

    virtual void impSetFlowOutlinerParams(SdrOutliner*, SdrOutliner*) override;

private:
    void impBroadcastCursorInfo() const;
};

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
