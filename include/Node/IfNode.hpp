/*
 * IfNode.h
 *
 *  Created on: 2014
 *      Author: jc
 */

#pragma once

#include <Context/json11.hpp>
#include <Node/Node.hpp>
#include <Context/Context.hpp>
#include <Exception.hpp>
#include <Parser/ExpressionParser.hpp>
#include <Parser/Fragment.hpp>

#include <algorithm>
#include <iterator>
#include <regex>
#include <set>
#include <typeinfo>

namespace RedZone
{
	class Writer;

	class IfNode : public Node
	{
	public:
		IfNode() : Node(true){}

		virtual void render(Writer * stream, Context * context) const
		{
			ExpressionParser parser(context);
			if (parser.parse(m_expression).bool_value())
			{
				renderChildren(stream, context, m_ifNodes);
			}
			else
			{
				renderChildren(stream, context, m_elseNodes);
			}
		}

		virtual void processFragment(Fragment const * fragment)
		{
			std::string clean = fragment->clean();
			m_expression.clear();
			m_ifNodes.clear();
			m_elseNodes.clear();
			std::copy(clean.begin() + 3, clean.end(), std::back_inserter(m_expression));
		}

		virtual void exitScope(std::string const & endTag)
		{
			if (endTag != "endif")
				throw TemplateSyntaxError(endTag);
			std::vector< std::shared_ptr< Node > > * current = &m_ifNodes;
			for (auto child : m_children)
			{
				if (child->name() == "Else")
				{
					current = &m_elseNodes;
					continue;
				}
				current->push_back(child);
			}
		}

		virtual std::string name() const{ return "If"; }

		virtual ~IfNode(){}

	protected:
		std::string m_expression;
		std::vector< std::shared_ptr< Node > > m_ifNodes;
		std::vector< std::shared_ptr< Node > > m_elseNodes;
	};

} /* namespace RedZone */

