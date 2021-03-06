/*
 * BlockNode.h
 *
 *      Author: jc
 */

#pragma once

#include <Exception.hpp>
#include <Parser/Fragment.hpp>
#include <Node/Node.hpp>

#include <regex>

namespace GreenZone
{

	class BlockNode : public Node
	{
	public:
		BlockNode()
			: Node(true)
		{}

		virtual void render(Writer * stream, Context * context) const
		{
			renderChildren(stream, context);
		}
		virtual void processFragment(Fragment const * fragment)
		{
			static std::regex const blockSplitter(R"(^block\s+(\w+)$)");
			std::smatch match;
			std::string cleaned = fragment->clean();
			if (!std::regex_match(cleaned, match, blockSplitter))
			{
				throw TemplateSyntaxError(fragment->clean());
			}
			m_blockName = match[1];
		}
		inline virtual void exitScope(std::string const & endTag)
		{
			if (endTag != "endblock")
				throw TemplateSyntaxError(endTag);
		}

		virtual std::string name() const
		{
			return "Block";
		}
		std::string blockName() const
		{
			return m_blockName;
		}

		virtual ~BlockNode(){}

	protected:
		std::string m_blockName;
	};

} /* namespace RedZone */

