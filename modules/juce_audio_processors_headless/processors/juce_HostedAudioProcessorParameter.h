/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-9-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

//==============================================================================
/**
    A parameter with functions that are useful for plugin hosts.

    @tags{Audio}
*/
struct JUCE_API  HostedAudioProcessorParameter : public AudioProcessorParameter
{
    using AudioProcessorParameter::AudioProcessorParameter;

    /** Returns an ID that is unique to this parameter.

        Parameter indices are unstable across plugin versions, which means that the
        parameter found at a particular index in one version of a plugin might move
        to a different index in the subsequent version.

        Unlike the parameter index, the ID returned by this function should be
        somewhat stable (depending on the format of the plugin), so it is more
        suitable for storing/recalling automation data.
    */
    virtual String getParameterID() const = 0;

    /** The range of the values this parameter takes, in the units the plugin
        itself publishes, when its format provides one.

        getValue() and setValue() always work in a normalised 0..1 range, no
        matter what the plugin does internally. Plugins that declare their own
        units — Hz, dB, seconds, and so on — publish a range too, and this is
        it: the normalised domain covers exactly this range.

        To present or accept a value in the plugin's units, convert it:

            auto native = param.convertFrom0to1 (param.getValue());        // show
            param.setValueNotifyingHost (param.convertTo0to1 (native));    // set

        getText() and getValueForText() stay in the normalised domain, so
        convert around them in the same way.

        The default implementation returns the identity range, 0..1, so a
        format that publishes nothing is unaffected — its normalised domain is
        also its value domain.

        @see convertTo0to1, convertFrom0to1
    */
    virtual const NormalisableRange<float>& getNormalisableRange() const
    {
        static const NormalisableRange<float> identity;
        return identity;
    }

    /** Normalises and snaps a value based on the normalisable range. */
    float convertTo0to1 (float v) const noexcept
    {
        const auto& range = getNormalisableRange();
        return range.convertTo0to1 (range.snapToLegalValue (v));
    }

    /** Denormalises and snaps a value based on the normalisable range. */
    float convertFrom0to1 (float v) const noexcept
    {
        const auto& range = getNormalisableRange();
        return range.snapToLegalValue (range.convertFrom0to1 (jlimit (0.0f, 1.0f, v)));
    }
};

} // namespace juce
