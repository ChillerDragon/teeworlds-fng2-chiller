/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef stats_h
#define stats_h

#include <engine/server.h>
#include <time.h>
#include "gamecontext.h"
#include <fcntl.h>
#include <unistd.h>

#define STATS_DIR "stats"

#define ADD_AVG(vnew,avg,nsamp) (((float)((vnew) + (float)((nsamp) * (avg)))) / (++nsamp))

#ifndef TEE_STATS
#define TEE_STATS
struct tee_stats {
	int spree, spree_max, multi, multis[6];
	int kills, kills_wrong, kills_x2;
	int lastkilltime, frozeby, deaths, steals, suicides;
	int shots, freezes, frozen, hammers, hammered, id;
	int num_samples;
	unsigned short avg_ping;
	unsigned char ping_tick, is_bot;
	int bounce_shots, tick_count;
	time_t join_time;
	int num_games, max_multi, version, stolen_from;
};
#endif

//#define PLAYER_ENTRY(pPlayer) find_round_entry(Server()->ClientName(pPlayer->GetCID()))
#define ID_NAME(id) (Server()->ClientName(id))
#define PLAYER_NUM(i) (GameServer()->m_apPlayers[i])

class tstats
{
	IServer *m_pServer;

	void send_stats (const char *name, int req_by, struct tee_stats *ct, int is_all);
	void print_best (const char *msg, int max, 
		double (*callback)(struct tee_stats, char *), int all);
	double print_best_group (char *dst, struct tee_stats *stats, char **names, 
		int num, double (*callback)(struct tee_stats, char *), double max);
	struct tee_stats read_statsfile (const char *name, time_t create);
	void top_special (const char *message, int ClientID);
	void stats_private (const char *name, int req_by, struct tee_stats *ct, int is_all);
	static double get_ping (struct tee_stats, char *);
	static double get_wrong (struct tee_stats, char *);
	static double get_steals (struct tee_stats, char *);
	static double get_neg_steals (struct tee_stats, char *);
	static double get_kd (struct tee_stats, char *);
	static double get_kd_all (struct tee_stats, char *);
	static double get_accuracy (struct tee_stats, char *);
	static double get_max_spree (struct tee_stats fstats, char *);
	static double get_max_multi (struct tee_stats fstats, char *);
	static double get_kills (struct tee_stats fstats, char *);
	static double get_hammers (struct tee_stats fstats, char *);
	static double get_bounces (struct tee_stats fstats, char *);
	static double get_suicides (struct tee_stats fstats, char *);
	static double get_accuracy_all (struct tee_stats fstats, char *);
	struct tee_stats *total_stats;
	char **total_names;
	int num_totals;
	int max_totals;
	
	int last_reqd;
	int last_reqds;
	
	struct tee_stats round_stats[512];
	char *round_names[512];
	int round_index;
	
	void SendChat(int ChatterClientID, int Team, const char *pText);
	void SendChatTarget(int To, const char *pText);

	CGameContext *game_server;
	const char *stat_dir;
public:
	IServer *Server() const { return m_pServer; }
	tstats(CGameContext *game_srv, const char *dir);
	~tstats();
	int ertimer;
	
	struct tee_stats *current[512];
	void do_kill_messages (struct tee_stats *, struct tee_stats *);
	void update_stats (struct tee_stats *dst, struct tee_stats *src);
	struct tee_stats *add_round_entry (struct tee_stats st, const char *name);
	struct tee_stats *find_round_entry (const char *name);
	struct tee_stats *find_total_entry (const char *name);
	void on_round_end (void);
	void on_namechange (int ClientID, const char *name);
	void on_msg (const char *message, int ClientID);
	void on_enter (int ClientID, const char *name);
	void on_drop (int ClientID, const char *pReason);
	void merge_into (const char *, const char *);
};

#endif
