#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>

namespace godot {

	class GD_Line : public Node2D
	{
		GODOT_CLASS(GD_Line, Node2D)

	public:
		static void _register_methods();

		GD_Line();
		~GD_Line();

		void _init(void); // our initializer called by Godot
		void _ready(void);
		void _process(float _delta);
		void _physics_process(float _delta);

		void init(Vector3 start, Vector3 end, Color color, float time);

	private:
		Vector3 m_start;
		Vector3 m_end;
		Color m_color;
		float m_time;
	};

}
