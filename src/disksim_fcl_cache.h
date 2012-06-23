    
/*
* Flash Cache Layer (FCL) (Version 1.0) 
*
* Author		: Yongseok Oh (ysoh@uos.ac.kr)
* Date			: 18/06/2012  
* Description	: 
*
*/

#ifndef _FCL_CACHE_H
#define _FCL_CACHE_H  

#include "../ssdmodel/ssd_utils.h"

#define HASH_NUM (16*1024)
#define BG_HASH_NUM (1024)
#define GRP2BLK 1024

#define FCL_CACHE_FLAG_FILLING 1 
#define FCL_CACHE_FLAG_SEALED 2  

struct cache_manager{

 listnode *cm_head;
 listnode **cm_hash;

/* 
 listnode *cm_hddrq;
 listnode *cm_hddwq;
 listnode *cm_ssdrq;
 listnode *cm_ssdwq;
*/ 

 listnode *cm_destage_ptr;
 unsigned int cm_hit;
 unsigned int cm_miss;
 unsigned int cm_ref;
 unsigned int cm_read_hit;
 unsigned int cm_read_ref;

 int cm_size; 
 int cm_free;
 int cm_count;
 int cm_max;
 int cm_min;
 char *cm_name;

 int cm_dirty_count;

 int cm_destage_count;
 int cm_stage_count;

 int cm_groupcount;
 int cm_lowwater;
 int cm_highwater;
 int cm_policy;

 void (*cache_open)(struct cache_manager *cache,int cache_size, int cache_max);
 void (*cache_close)(struct cache_manager *cache, int print);
 listnode *(*cache_presearch)(struct cache_manager *cache, unsigned int blkno);
 listnode *(*cache_search)(struct cache_manager *cache,unsigned int blkno);
 void *(*cache_replace)(struct cache_manager *cache,int w); 
 void *(*cache_remove)(struct cache_manager *cache, void *node); 
 void (*cache_insert)(struct cache_manager *cache, void * node);
 void *(*cache_alloc)(void *node, unsigned int blkno);
 int (*cache_inc)(struct cache_manager *cache, int i);
 int (*cache_dec)(struct cache_manager *cache, int i);
 //int (*cache_makerq) (struct cache_manager *cache, listnode *rqlist, Rb_node *tree, int blkno);
 //void (*cache_flushrq) (struct cache_manager *cache, int dev, int rw, listnode *Qlist);
 //int (*cache_releaserq) (struct cache_manager *cache, listnode *rqlist);
};

struct lru_node{
	listnode *cn_node;
	listnode *cn_hash;
	unsigned int cn_blkno;
	int cn_ssd_blk;
	int cn_dirty;
	int cn_read;
	int cn_flag;
	unsigned int cn_recency;
	unsigned int cn_frequency;
	void *cn_temp1;
	void *cn_temp2;
};


#if 0 
struct ghost_node{
	listnode *gn_node;
	listnode *gn_hash;
	unsigned int gn_blkno;	
	int gn_ssd_blk;
	int gn_dirty;
	unsigned int gn_recency;
	unsigned int gn_frequency;
};

struct clock_node{
	listnode *cn_node;
	listnode *cn_hash;
	unsigned int cn_blkno;
	int cn_ssd_blk;		
	int cn_dirty;
	int cn_read;
	unsigned int cn_recency;
	unsigned int cn_frequency;
};
#endif 


#if 0
struct bglru_node{
	listnode *bg_node;
	listnode *bg_hash;
	unsigned int bg_no;	/* block group number */
	int block_count;
	int bg_recency;
	listnode *bg_block_list;
	listnode *bg_block_hash[BG_HASH_NUM];
};

struct seq_node{
	listnode *sq_node;
	listnode *sq_hash;
	unsigned int sq_no;	/* block group number */
	int sq_block_count;
	int sq_dirty_count;
	int sq_ref;
	int sq_hit;
	double sq_start_time;
	listnode *sq_block_list;
	listnode *sq_block_hash[BG_HASH_NUM];
};
#endif 

#define CACHE_OPEN(c, sz, m) c->cache_open((struct cache_manager *)c, sz, m)
#define CACHE_CLOSE(c, print ) c->cache_close((struct cache_manager *)c, print)
#define CACHE_PRESEARCH(c, p) c->cache_presearch((struct cache_manager *)c, p)
#define CACHE_SEARCH(c, p) c->cache_search((struct cache_manager *)c, p)
#define CACHE_REPLACE(c, w) c->cache_replace((struct cache_manager *)c, w)
#define CACHE_REMOVE(c, p) c->cache_remove((struct cache_manager *)c, p)
#define CACHE_INSERT(c, p) c->cache_insert((struct cache_manager *)c, p)
#define CACHE_ALLOC(c, n, p) c->cache_alloc(n, p)
#define CACHE_INC(c, i) c->cache_inc((struct cache_manager *)c, i)
#define CACHE_DEC(c, i) c->cache_dec((struct cache_manager *)c, i)
#define CACHE_HIT(c) ((float)c->cm_hit/c->cm_ref)
#define CACHE_MAKERQ(c, p, r, i) c->cache_makerq((struct cache_manager *)c, p, r, i)
#define CACHE_RELEASERQ(c, p) c->cache_releaserq((struct cache_manager *)c, p)
#define CACHE_FLUSHRQ(c, d, r, l) c->cache_flushrq((struct cache_manager *)c, d, r, l)


#define CACHE_LRU_RW 0
#define CACHE_LRU_RWO_ADAPTIVE 1
#define CACHE_BGCLOCK_RW 2
#define CACHE_BGCLOCK_RWO_ADAPTIVE 3
#define CACHE_WORKLOAD_ANALYSIS 4
#define CACHE_DISK_ONLY 5
#define CACHE_SSD_ONLY 6
#define CACHE_LRU_RW_ADAPTIVE 7
#define CACHE_ZIPF 8
#define CACHE_LRU_SELECTIVE 9
#define CACHE_POLICY_NUM 10


void lru_open(struct cache_manager *c,int cache_size, int cache_max);
void lru_close(struct cache_manager *c, int print);
listnode *lru_presearch(struct cache_manager *c, unsigned int blkno);
listnode *lru_search(struct cache_manager *c,unsigned int blkno);
void *lru_remove(struct cache_manager *c, listnode *remove_ptr);
void *lru_alloc(struct lru_node *ln, unsigned int blkno);
void lru_insert(struct cache_manager *c,struct lru_node *ln);
listnode *lru_replace(struct cache_manager *c, int watermark);	
int lru_inc(struct cache_manager *c, int inc_val);
int lru_dec(struct cache_manager *c, int dec_val);
void lru_init(struct cache_manager **c,char *name, int size,int max_sz,int high,int low);

#endif 
