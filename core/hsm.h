#pragma once


namespace sst
{
	class hsm_owner
	{
	public:
		hsm_owner() {}
		virtual ~hsm_owner() {}
	};

	class hsm final
	{
	public:
		class state
		{
		public:
			struct node
			{
				enum Code
				{
					NOOP = 1,
					EXIT,
					ENTER,
					TARGET
				};
				Code code{ NOOP };
				state* state{ nullptr };
			};
			typedef std::vector<node> path;

			state();
			virtual ~state();

			virtual state* create() { return nullptr; }

			bool init(hsm* hsm, size_t id, state* parent);

			void on_enter();
			state* on_tick(uint32 age);
			void on_exit();

			void finish();
			bool is_transable(size_t target_state);
			bool has_state(size_t id) const;
			size_t get_id() const { return id_; }
			state* get_state(size_t id);
			state* get_parent() const { return parent_; }
			bool get_transition_path_to(size_t target, path& path);
			hsm_owner* get_owner();

		protected:
			typedef std::vector<state*> state_list;
			
			virtual bool _Init();
			virtual void _Fini();
			virtual void _On_enter();
			virtual state* _On_tick(uint32 age);
			virtual void _On_exit();
			virtual bool _Is_transable(size_t target_state);
			void _Attach(state* state);
			bool _Get_child_path_to(size_t target, path& path);

			hsm* hsm_{ nullptr };
			size_t id_{ 0 };
			state* parent_{ nullptr };
			size_t target_id_{ 0 };
			state_list children_{};
		};

		hsm();
		~hsm();

		enum state_type
		{
			state_root = 1,
			state_begin
		};

		bool start(hsm_owner* owner);
		void on_tick(uint32 age);
		void tran(state_type state);
		void finish();
		
		[[nodiscard]] hsm_owner* get_owner() const { return owner_; }
		[[nodiscard]] state* get_current() const { return current_; }
		state* get_state(const state_type type);
		state* get_root() { return &root_; }

	protected:
		hsm_owner* owner_{ nullptr };
		state root_{};
		state* current_{ nullptr };
	};
}