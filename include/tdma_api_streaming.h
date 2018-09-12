/*
Copyright (C) 2018 Jonathon Ogden <jeog.dev@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
*/

#ifndef TDMA_API_STREAMING_H
#define TDMA_API_STREAMING_H

#include "_common.h"
#include "tdma_common.h"

#ifdef __cplusplus
#include <map>
#include <set>
#include <memory>
#include <thread>
#include <string>

#include "json.hpp"
#include "websocket_connect.h"
#include "threadsafe_hashmap.h"

using json = nlohmann::json;
#endif /* __cplusplus */


#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamerServiceType, n)
DECL_C_CPP_TDMA_ENUM(StreamerServiceType, 1, 19,
    BUILD_ENUM_NAME( NONE ),
    BUILD_ENUM_NAME( QUOTE ),
    BUILD_ENUM_NAME( OPTION ),
    BUILD_ENUM_NAME( LEVELONE_FUTURES ),
    BUILD_ENUM_NAME( LEVELONE_FOREX ),
    BUILD_ENUM_NAME( LEVELONE_FUTURES_OPTIONS ),
    BUILD_ENUM_NAME( NEWS_HEADLINE ),
    BUILD_ENUM_NAME( CHART_EQUITY ),
    BUILD_ENUM_NAME( CHART_FOREX ), /* NOT WORKING */
    BUILD_ENUM_NAME( CHART_FUTURES ),
    BUILD_ENUM_NAME( CHART_OPTIONS ),
    BUILD_ENUM_NAME( TIMESALE_EQUITY ),
    BUILD_ENUM_NAME( TIMESALE_FOREX ), /* NOT WORKING */
    BUILD_ENUM_NAME( TIMESALE_FUTURES ),
    BUILD_ENUM_NAME( TIMESALE_OPTIONS ),
    BUILD_ENUM_NAME( ACTIVES_NASDAQ ),
    BUILD_ENUM_NAME( ACTIVES_NYSE ),
    BUILD_ENUM_NAME( ACTIVES_OTCBB ),
    BUILD_ENUM_NAME( ACTIVES_OPTIONS ),
    BUILD_ENUM_NAME( ADMIN ) // <- NOTE THIS DOESNT MATCH TYPE_ID_SUB_[] consts
    /* NOT IMPLEMENTED YET */
    //BUILD_ENUM_NAME( CHART_HISTORY_FUTURES),
    //BUILD_ENUM_NAME( ACCT_ACTIVITY),
    /* NOT DOCUMENTED BY TDMA */
    //BUILD_ENUM_NAME( FOREX_BOOK,
    //BUILD_ENUM_NAME( FUTURES_BOOK),
    //BUILD_ENUM_NAME( LISTED_BOOK),
    //BUILD_ENUM_NAME( NASDAQ_BOOK),
    //BUILD_ENUM_NAME( OPTIONS_BOOK),
    //BUILD_ENUM_NAME( FUTURES_OPTION_BOOK),
    //BUILD_ENUM_NAME( NEWS_STORY),
    //BUILD_ENUM_NAME( NEWS_HEADLINE_LIST),
    /* OLD API ? */
    //BUILD_ENUM_NAME( STREAMER_SERVER)
    );
#undef BUILD_ENUM_NAME

DECL_C_CPP_TDMA_ENUM(AdminCommandType, 0, 2,
    BUILD_C_CPP_TDMA_ENUM_NAME(AdminCommandType, LOGIN),
    BUILD_C_CPP_TDMA_ENUM_NAME(AdminCommandType, LOGOUT),
    BUILD_C_CPP_TDMA_ENUM_NAME(AdminCommandType, QOS)
    );

DECL_C_CPP_TDMA_ENUM(QOSType, 0, 5,
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, express),   /* 500 ms */
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, real_time), /* 750 ms */
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, fast),      /* 1000 ms DEFAULT */
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, moderate),  /* 1500 ms */
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, slow),      /* 3000 ms */
    BUILD_C_CPP_TDMA_ENUM_NAME(QOSType, delayed)    /* 5000 ms */
    );

#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(QuotesSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(QuotesSubscriptionField, 0, 52,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(bid_price),
    BUILD_ENUM_NAME(ask_price),
    BUILD_ENUM_NAME(last_price),
    BUILD_ENUM_NAME(bid_size),
    BUILD_ENUM_NAME(ask_size),
    BUILD_ENUM_NAME(ask_id),
    BUILD_ENUM_NAME(bid_id),
    BUILD_ENUM_NAME(total_volume),
    BUILD_ENUM_NAME(last_size),
    BUILD_ENUM_NAME(trade_time),
    BUILD_ENUM_NAME(quote_time),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(bid_tick),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(exchange_id),
    BUILD_ENUM_NAME(marginable),
    BUILD_ENUM_NAME(shortable),
    BUILD_ENUM_NAME(island_bid),
    BUILD_ENUM_NAME(island_ask),
    BUILD_ENUM_NAME(island_volume),
    BUILD_ENUM_NAME(quote_day),
    BUILD_ENUM_NAME(trade_day),
    BUILD_ENUM_NAME(volatility),
    BUILD_ENUM_NAME(description),
    BUILD_ENUM_NAME(last_id),
    BUILD_ENUM_NAME(digits),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(net_change),
    BUILD_ENUM_NAME(high_52_week),
    BUILD_ENUM_NAME(low_52_week),
    BUILD_ENUM_NAME(pe_ratio),
    BUILD_ENUM_NAME(dividend_amount),
    BUILD_ENUM_NAME(dividend_yeild),
    BUILD_ENUM_NAME(island_bid_size),
    BUILD_ENUM_NAME(island_ask_size),
    BUILD_ENUM_NAME(nav),
    BUILD_ENUM_NAME(fund_price),
    BUILD_ENUM_NAME(exchanged_name),
    BUILD_ENUM_NAME(dividend_date),
    BUILD_ENUM_NAME(regular_market_quote),
    BUILD_ENUM_NAME(regular_market_trade),
    BUILD_ENUM_NAME(regular_market_last_price),
    BUILD_ENUM_NAME(regular_market_last_size),
    BUILD_ENUM_NAME(regular_market_trade_time),
    BUILD_ENUM_NAME(regular_market_trade_day),
    BUILD_ENUM_NAME(regular_market_net_change),
    BUILD_ENUM_NAME(security_status),
    BUILD_ENUM_NAME(mark),
    BUILD_ENUM_NAME(quote_time_as_long),
    BUILD_ENUM_NAME(trade_time_as_long),
    BUILD_ENUM_NAME(regular_market_trade_time_as_long)
    );
#undef BUILD_ENUM_NAME

DECL_C_CPP_TDMA_ENUM(DurationType, 0, 5,
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, all_day),
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, min_60),
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, min_30),
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, min_10),
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, min_5),
    BUILD_C_CPP_TDMA_ENUM_NAME(DurationType, min_1)
    );

DECL_C_CPP_TDMA_ENUM(VenueType, 0, 5,
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, opts),
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, calls),
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, puts),
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, opts_desc),
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, calls_desc),
    BUILD_C_CPP_TDMA_ENUM_NAME(VenueType, puts_desc)
    );

#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(OptionsSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(OptionsSubscriptionField, 0, 41,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(description),
    BUILD_ENUM_NAME(bid_price),
    BUILD_ENUM_NAME(ask_price),
    BUILD_ENUM_NAME(last_price),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(total_volume),
    BUILD_ENUM_NAME(open_interest),
    BUILD_ENUM_NAME(volatility),
    BUILD_ENUM_NAME(quote_time),
    BUILD_ENUM_NAME(trade_time),
    BUILD_ENUM_NAME(money_intrinsic_value),
    BUILD_ENUM_NAME(quote_day),
    BUILD_ENUM_NAME(trade_day),
    BUILD_ENUM_NAME(expiration_year),
    BUILD_ENUM_NAME(multiplier),
    BUILD_ENUM_NAME(digits),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(bid_size),
    BUILD_ENUM_NAME(ask_size),
    BUILD_ENUM_NAME(last_size),
    BUILD_ENUM_NAME(net_change),
    BUILD_ENUM_NAME(strike_price),
    BUILD_ENUM_NAME(contract_type),
    BUILD_ENUM_NAME(underlying),
    BUILD_ENUM_NAME(expiration_month),
    BUILD_ENUM_NAME(deliverables),
    BUILD_ENUM_NAME(time_value),
    BUILD_ENUM_NAME(expiration_day),
    BUILD_ENUM_NAME(days_to_expiration),
    BUILD_ENUM_NAME(delta),
    BUILD_ENUM_NAME(gamma),
    BUILD_ENUM_NAME(theta),
    BUILD_ENUM_NAME(vega),
    BUILD_ENUM_NAME(rho),
    BUILD_ENUM_NAME(security_status),
    BUILD_ENUM_NAME(theoretical_option_value),
    BUILD_ENUM_NAME(underlying_price),
    BUILD_ENUM_NAME(uv_expiration_type),
    BUILD_ENUM_NAME(mark)
    );
#undef BUILD_ENUM_NAME

#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(LevelOneFuturesSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(LevelOneFuturesSubscriptionField, 0, 35,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(bid_price),
    BUILD_ENUM_NAME(ask_price),
    BUILD_ENUM_NAME(last_price),
    BUILD_ENUM_NAME(bid_size),
    BUILD_ENUM_NAME(ask_size),
    BUILD_ENUM_NAME(ask_id),
    BUILD_ENUM_NAME(bid_id),
    BUILD_ENUM_NAME(total_volume),
    BUILD_ENUM_NAME(last_size),
    BUILD_ENUM_NAME(quote_time),
    BUILD_ENUM_NAME(trade_time),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(exchange_id),
    BUILD_ENUM_NAME(description),
    BUILD_ENUM_NAME(last_id),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(net_change),
    BUILD_ENUM_NAME(future_percent_change),
    BUILD_ENUM_NAME(exchange_name),
    BUILD_ENUM_NAME(security_status),
    BUILD_ENUM_NAME(open_interest),
    BUILD_ENUM_NAME(mark),
    BUILD_ENUM_NAME(tick),
    BUILD_ENUM_NAME(tick_amount),
    BUILD_ENUM_NAME(product),
    BUILD_ENUM_NAME(future_price_format),
    BUILD_ENUM_NAME(future_trading_hours),
    BUILD_ENUM_NAME(future_is_tradable),
    BUILD_ENUM_NAME(future_multiplier),
    BUILD_ENUM_NAME(future_is_active),
    BUILD_ENUM_NAME(future_settlement_price),
    BUILD_ENUM_NAME(future_active_symbol),
    BUILD_ENUM_NAME(future_expiration_date)
    );
#undef BUILD_ENUM_NAME

#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(LevelOneForexSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(LevelOneForexSubscriptionField, 0, 29,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(bid_price),
    BUILD_ENUM_NAME(ask_price),
    BUILD_ENUM_NAME(last_price),
    BUILD_ENUM_NAME(bid_size),
    BUILD_ENUM_NAME(ask_size),
    BUILD_ENUM_NAME(total_volume),
    BUILD_ENUM_NAME(last_size),
    BUILD_ENUM_NAME(quote_time),
    BUILD_ENUM_NAME(trade_time),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(exchange_id),
    BUILD_ENUM_NAME(description),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(net_change),
    BUILD_ENUM_NAME(percent_change),
    BUILD_ENUM_NAME(exchange_name),
    BUILD_ENUM_NAME(digits),
    BUILD_ENUM_NAME(security_status),
    BUILD_ENUM_NAME(tick),
    BUILD_ENUM_NAME(tick_amount),
    BUILD_ENUM_NAME(product),
    BUILD_ENUM_NAME(trading_hours),
    BUILD_ENUM_NAME(is_tradable),
    BUILD_ENUM_NAME(market_maker),
    BUILD_ENUM_NAME(high_52_week),
    BUILD_ENUM_NAME(low_52_week),
    BUILD_ENUM_NAME(mark)
    );
#undef BUILD_ENUM_NAME



#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(LevelOneFuturesOptionsSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(LevelOneFuturesOptionsSubscriptionField, 0, 35,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(bid_price),
    BUILD_ENUM_NAME(ask_price),
    BUILD_ENUM_NAME(last_price),
    BUILD_ENUM_NAME(bid_size),
    BUILD_ENUM_NAME(ask_size),
    BUILD_ENUM_NAME(ask_id),
    BUILD_ENUM_NAME(bid_id),
    BUILD_ENUM_NAME(total_volume),
    BUILD_ENUM_NAME(last_size),
    BUILD_ENUM_NAME(quote_time),
    BUILD_ENUM_NAME(trade_time),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(exchange_id),
    BUILD_ENUM_NAME(description),
    BUILD_ENUM_NAME(last_id),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(net_change),
    BUILD_ENUM_NAME(future_percent_change),
    BUILD_ENUM_NAME(exchange_name),
    BUILD_ENUM_NAME(security_status),
    BUILD_ENUM_NAME(open_interest),
    BUILD_ENUM_NAME(mark),
    BUILD_ENUM_NAME(tick),
    BUILD_ENUM_NAME(tick_amount),
    BUILD_ENUM_NAME(product),
    BUILD_ENUM_NAME(future_price_format),
    BUILD_ENUM_NAME(future_trading_hours),
    BUILD_ENUM_NAME(future_is_tradable),
    BUILD_ENUM_NAME(future_multiplier),
    BUILD_ENUM_NAME(future_is_active),
    BUILD_ENUM_NAME(future_settlement_price),
    BUILD_ENUM_NAME(future_active_symbol),
    BUILD_ENUM_NAME(future_expiration_date)
    );
#undef BUILD_ENUM_NAME


#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(NewsHeadlineSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(NewsHeadlineSubscriptionField, 0, 10,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(error_code),
    BUILD_ENUM_NAME(story_datetime),
    BUILD_ENUM_NAME(headline_id),
    BUILD_ENUM_NAME(status),
    BUILD_ENUM_NAME(headline),
    BUILD_ENUM_NAME(story_id),
    BUILD_ENUM_NAME(count_for_keyword),
    BUILD_ENUM_NAME(keyword_array),
    BUILD_ENUM_NAME(is_host),
    BUILD_ENUM_NAME(story_source)
    );
#undef BUILD_ENUM_NAME


#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(ChartEquitySubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(ChartEquitySubscriptionField, 0, 8,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(volume),
    BUILD_ENUM_NAME(sequence),
    BUILD_ENUM_NAME(chart_time),
    BUILD_ENUM_NAME(chart_day)
    );
#undef BUILD_ENUM_NAME

#define BUILD_ENUM_NAME(n) \
    BUILD_C_CPP_TDMA_ENUM_NAME(ChartSubscriptionField, n)
DECL_C_CPP_TDMA_ENUM(ChartSubscriptionField, 0, 6,
    BUILD_ENUM_NAME(symbol),
    BUILD_ENUM_NAME(chart_time),
    BUILD_ENUM_NAME(open_price),
    BUILD_ENUM_NAME(high_price),
    BUILD_ENUM_NAME(low_price),
    BUILD_ENUM_NAME(close_price),
    BUILD_ENUM_NAME(volume)
    );
#undef BUILD_ENUM_NAME

DECL_C_CPP_TDMA_ENUM(TimesaleSubscriptionField, 0, 5,
    BUILD_C_CPP_TDMA_ENUM_NAME(TimesaleSubscriptionField, symbol),
    BUILD_C_CPP_TDMA_ENUM_NAME(TimesaleSubscriptionField, trade_time),
    BUILD_C_CPP_TDMA_ENUM_NAME(TimesaleSubscriptionField, last_price),
    BUILD_C_CPP_TDMA_ENUM_NAME(TimesaleSubscriptionField, last_size),
    BUILD_C_CPP_TDMA_ENUM_NAME(TimesaleSubscriptionField, last_sequence)
    );

DECL_C_CPP_TDMA_ENUM(StreamingCallbackType, 0, 6,
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, listening_start),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, listening_stop),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, data),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, request_response),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, notify),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, timeout),
    BUILD_C_CPP_TDMA_ENUM_NAME(StreamingCallbackType, error)
    );


static const int SUBSCRIPTION_MAX_FIELDS = 100;
static const int SUBSCRIPTION_MAX_SYMBOLS = 5000;

static const int TYPE_ID_SUB_QUOTES = 1;
static const int TYPE_ID_SUB_OPTIONS = 2;
static const int TYPE_ID_SUB_LEVEL_ONE_FUTURES = 3;
static const int TYPE_ID_SUB_LEVEL_ONE_FOREX = 4;
static const int TYPE_ID_SUB_LEVEL_ONE_FUTURES_OPTIONS = 5;
static const int TYPE_ID_SUB_NEWS_HEADLINE = 6;
static const int TYPE_ID_SUB_CHART_EQUITY = 7;
static const int TYPE_ID_SUB_CHART_FOREX = 8; // not working
static const int TYPE_ID_SUB_CHART_FUTURES = 9;
static const int TYPE_ID_SUB_CHART_OPTIONS = 10;
static const int TYPE_ID_SUB_TIMESALE_EQUITY = 11;
static const int TYPE_ID_SUB_TIMESALE_FOREX = 12; // not working
static const int TYPE_ID_SUB_TIMESALE_FUTURES = 13;
static const int TYPE_ID_SUB_TIMESALE_OPTIONS = 14;
static const int TYPE_ID_SUB_ACTIVES_NASDAQ = 15;
static const int TYPE_ID_SUB_ACTIVES_NYSE = 16;
static const int TYPE_ID_SUB_ACTIVES_OTCBB = 17;
static const int TYPE_ID_SUB_ACTIVES_OPTION = 18;


#define DECL_CSUB_STRUCT(name) typedef struct{ void *obj; int type_id; } name

DECL_CSUB_STRUCT(QuotesSubscription_C);
DECL_CSUB_STRUCT(OptionsSubscription_C);
DECL_CSUB_STRUCT(LevelOneFuturesSubscription_C);
DECL_CSUB_STRUCT(LevelOneForexSubscription_C);
DECL_CSUB_STRUCT(LevelOneFuturesOptionsSubscription_C);
DECL_CSUB_STRUCT(NewsHeadlineSubscription_C);
DECL_CSUB_STRUCT(ChartEquitySubscription_C);
//DECL_CSUB_STRUCT(ChartForexSubscription_C);
DECL_CSUB_STRUCT(ChartFuturesSubscription_C);
DECL_CSUB_STRUCT(ChartOptionsSubscription_C);
DECL_CSUB_STRUCT(TimesaleEquitySubscription_C);
//DECL_CSUB_STRUCT(TimesalesForexSubscription_C);
DECL_CSUB_STRUCT(TimesaleFuturesSubscription_C);
DECL_CSUB_STRUCT(TimesaleOptionsSubscription_C);
DECL_CSUB_STRUCT(NasdaqActivesSubscription_C);
DECL_CSUB_STRUCT(NYSEActivesSubscription_C);
DECL_CSUB_STRUCT(OTCBBActivesSubscription_C);
DECL_CSUB_STRUCT(OptionActivesSubscription_C);
#undef DECL_CSUB_STRUCT

/* SUBSCRIPTION CREATE METHODS */

#define DECL_CSUB_FIELD_SYM_CREATE_FUNC(name) \
EXTERN_C_SPEC_ DLL_SPEC_ int \
name##_Create_ABI( const char **symbols, size_t nsymbols, int *fields, \
                   size_t nfields, name##_C *psub, int allow_exceptions )

#define DECL_CSUB_DURATION_CREATE_FUNC(name) \
EXTERN_C_SPEC_ DLL_SPEC_ int \
name##_Create_ABI( int duration_type, name##_C *psub, int allow_exceptions )

/* Create methods for subs that takes symbols and fields */
DECL_CSUB_FIELD_SYM_CREATE_FUNC(QuotesSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(OptionsSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneForexSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(NewsHeadlineSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartEquitySubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartFuturesSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartOptionsSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleEquitySubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleOptionsSubscription);
#undef DECL_CSUB_FIELD_SYM_CREATE_FUNC

/* Create methods for subs that take duration */
DECL_CSUB_DURATION_CREATE_FUNC(NasdaqActivesSubscription);
DECL_CSUB_DURATION_CREATE_FUNC(NYSEActivesSubscription);
DECL_CSUB_DURATION_CREATE_FUNC(OTCBBActivesSubscription);
#undef DECL_CSUB_DURATION_CREATE_FUNC

EXTERN_C_SPEC_ DLL_SPEC_ int
OptionActivesSubscription_Create_ABI( int venue,
                                           int duration_type,
                                           OptionActivesSubscription_C *psub,
                                           int allow_exceptions );


/* SUBSCRIPTION DESTROY METHODS */

#define DECL_CSUB_DESTROY_FUNC(name) \
EXTERN_C_SPEC_ DLL_SPEC_ int \
name##_Destroy_ABI(name##_C *psub, int allow_exceptions)

/* Destroy for each subscription type */
DECL_CSUB_DESTROY_FUNC(QuotesSubscription);
DECL_CSUB_DESTROY_FUNC(OptionsSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneForexSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(NewsHeadlineSubscription);
DECL_CSUB_DESTROY_FUNC(ChartEquitySubscription);
DECL_CSUB_DESTROY_FUNC(ChartFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(ChartOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleEquitySubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(NasdaqActivesSubscription);
DECL_CSUB_DESTROY_FUNC(NYSEActivesSubscription);
DECL_CSUB_DESTROY_FUNC(OTCBBActivesSubscription);
DECL_CSUB_DESTROY_FUNC(OptionActivesSubscription);
#undef DECL_CSUB_DESTROY_FUNC

/* Generic destroy (cast to StreamingSubscription_C*) */
EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSubscription_Destroy_ABI(StreamingSubscription_C* psub,
                                      int allow_exceptions);


/* SUBSCRIPTION GET METHODS */

/* StreamingSubscription Base Methods */
EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSubscription_GetService_ABI( StreamingSubscription_C *psub,
                                          int *service,
                                          int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSubscription_GetCommand_ABI( StreamingSubscription_C *psub,
                                          char **buf,
                                          size_t *n,
                                          int allow_exceptions );

/* SubscriptionBySymbolBase Base Methods */
EXTERN_C_SPEC_ DLL_SPEC_ int
SubscriptionBySymbolBase_GetSymbols_ABI( StreamingSubscription_C *psub,
                                              char ***buffers,
                                              size_t *n,
                                              int allow_exceptions );

#define DECL_CSUB_GET_FIELDS_FUNC(name) \
EXTERN_C_SPEC_ DLL_SPEC_ int \
name##_GetFields_ABI(name##_C *psub, int **fields, size_t *n, int allow_exceptions);

#define DECL_CSUB_GET_FIELDS_BASE_FUNC(name) \
EXTERN_C_SPEC_ DLL_SPEC_ int \
name##_GetFields_ABI(StreamingSubscription_C *psub, int **fields, size_t *n, \
                     int allow_exceptions);

/* Get fields methods in derived */
DECL_CSUB_GET_FIELDS_FUNC(QuotesSubscription);
DECL_CSUB_GET_FIELDS_FUNC(OptionsSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneForexSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_GET_FIELDS_FUNC(NewsHeadlineSubscription);
DECL_CSUB_GET_FIELDS_FUNC(ChartEquitySubscription);
#undef DECL_CSUB_GET_FIELDS_FUNC

/* Get fields methods in base */
DECL_CSUB_GET_FIELDS_BASE_FUNC(ChartSubscriptionBase);
DECL_CSUB_GET_FIELDS_BASE_FUNC(TimesaleSubscriptionBase);
#undef DECL_CSUB_GET_FIELDS_BASE_FUNC

/* Get duration in base */
EXTERN_C_SPEC_ DLL_SPEC_ int
ActivesSubscriptionBase_GetDuration_ABI(StreamingSubscription_C *psub,
                                             int *duration,
                                             int allow_exceptions);

EXTERN_C_SPEC_ DLL_SPEC_ int
OptionActivesSubscription_GetVenue_ABI(OptionActivesSubscription_C *psub,
                                            int *venue,
                                            int allow_exceptions);

#ifndef __cplusplus

/* C Interface */

/* SUBSCRIPTION CREATE METHODS */

#define DECL_CSUB_FIELD_SYM_CREATE_FUNC(name, fname) \
static inline int \
name##_Create( const char **symbols, size_t nsymbols, fname *fields, \
               size_t nfields, name##_C *psub ) \
{ return name##_Create_ABI(symbols, nsymbols, (int*)fields, nfields, psub, 0); }

#define DECL_CSUB_DURATION_CREATE_FUNC(name) \
static inline int \
name##_Create( DurationType duration_type, name##_C *psub) \
{ return name##_Create_ABI((int)duration_type, psub, 0); }

/* Create methods for subs that takes symbols and fields */
DECL_CSUB_FIELD_SYM_CREATE_FUNC(QuotesSubscription,
    QuotesSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(OptionsSubscription,
    OptionsSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneFuturesSubscription,
    LevelOneFuturesSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneForexSubscription,
    LevelOneForexSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(LevelOneFuturesOptionsSubscription,
    LevelOneFuturesOptionsSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(NewsHeadlineSubscription,
    NewsHeadlineSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartEquitySubscription,
    ChartEquitySubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartFuturesSubscription,
    ChartSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(ChartOptionsSubscription,
    ChartSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleEquitySubscription,
    TimesaleSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleFuturesSubscription,
    TimesaleSubscriptionField);
DECL_CSUB_FIELD_SYM_CREATE_FUNC(TimesaleOptionsSubscription,
    TimesaleSubscriptionField);
#undef DECL_CSUB_FIELD_SYM_CREATE_FUNC

/* Create methods for subs that take duration */
DECL_CSUB_DURATION_CREATE_FUNC(NasdaqActivesSubscription);
DECL_CSUB_DURATION_CREATE_FUNC(NYSEActivesSubscription);
DECL_CSUB_DURATION_CREATE_FUNC(OTCBBActivesSubscription);
#undef DECL_CSUB_DURATION_CREATE_FUNC

static inline int
OptionActivesSubscription_Create( VenueType venue,
                                  DurationType duration_type,
                                  OptionActivesSubscription_C *psub)
{ return OptionActivesSubscription_Create_ABI((int)venue, (int)duration_type,
                                               psub, 0); }


/* SUBSCRIPTION DESTROY METHODS */

#define DECL_CSUB_DESTROY_FUNC(name) \
static inline int \
name##_Destroy(name##_C *psub) \
{ return name##_Destroy_ABI(psub, 0); }

/* Destroy for each subscription type */
DECL_CSUB_DESTROY_FUNC(QuotesSubscription);
DECL_CSUB_DESTROY_FUNC(OptionsSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneForexSubscription);
DECL_CSUB_DESTROY_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(NewsHeadlineSubscription);
DECL_CSUB_DESTROY_FUNC(ChartEquitySubscription);
DECL_CSUB_DESTROY_FUNC(ChartFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(ChartOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleEquitySubscription);
DECL_CSUB_DESTROY_FUNC(TimesaleOptionsSubscription);
DECL_CSUB_DESTROY_FUNC(NasdaqActivesSubscription);
DECL_CSUB_DESTROY_FUNC(NYSEActivesSubscription);
DECL_CSUB_DESTROY_FUNC(OTCBBActivesSubscription);
DECL_CSUB_DESTROY_FUNC(OptionActivesSubscription);
#undef DECL_CSUB_DESTROY_FUNC

/* Generic destroy (cast to StreamingSubscription_C*) */
static inline int
StreamingSubscription_Destroy(StreamingSubscription_C* psub)
{ return StreamingSubscription_Destroy_ABI(psub, 0); }


/* SUBSCRIPTION GET METHODS */

#define DECL_CSUB_GET_SERVICE_FUNC(name) \
static inline int \
name##_GetService( name##_C *psub, StreamerServiceType *service ) \
{ return StreamingSubscription_GetService_ABI( (StreamingSubscription_C*)psub, \
                                               (int*)service, 0); }

/* GetService methods for each sub type */
DECL_CSUB_GET_SERVICE_FUNC(QuotesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(OptionsSubscription);
DECL_CSUB_GET_SERVICE_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(LevelOneForexSubscription);
DECL_CSUB_GET_SERVICE_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_GET_SERVICE_FUNC(NewsHeadlineSubscription);
DECL_CSUB_GET_SERVICE_FUNC(ChartEquitySubscription);
DECL_CSUB_GET_SERVICE_FUNC(ChartFuturesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(ChartOptionsSubscription);
DECL_CSUB_GET_SERVICE_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(TimesaleEquitySubscription);
DECL_CSUB_GET_SERVICE_FUNC(TimesaleOptionsSubscription);
DECL_CSUB_GET_SERVICE_FUNC(NasdaqActivesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(NYSEActivesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(OTCBBActivesSubscription);
DECL_CSUB_GET_SERVICE_FUNC(OptionActivesSubscription);
/* GetService generic method (cast to StreamerSubscription_C*) */
DECL_CSUB_GET_SERVICE_FUNC(StreamingSubscription);
#undef DECL_CSUB_GET_SERVICE_FUNC


#define DECL_CSUB_GET_COMMAND_FUNC(name) \
static inline int \
name##_GetCommand( name##_C *psub, char **buf, size_t *n ) \
{ return StreamingSubscription_GetCommand_ABI( (StreamingSubscription_C*)psub, \
                                               buf, n, 0); }

/* GetCommand methods for each sub type */
DECL_CSUB_GET_COMMAND_FUNC(QuotesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(OptionsSubscription);
DECL_CSUB_GET_COMMAND_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(LevelOneForexSubscription);
DECL_CSUB_GET_COMMAND_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_GET_COMMAND_FUNC(NewsHeadlineSubscription);
DECL_CSUB_GET_COMMAND_FUNC(ChartEquitySubscription);
DECL_CSUB_GET_COMMAND_FUNC(ChartFuturesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(ChartOptionsSubscription);
DECL_CSUB_GET_COMMAND_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(TimesaleEquitySubscription);
DECL_CSUB_GET_COMMAND_FUNC(TimesaleOptionsSubscription);
DECL_CSUB_GET_COMMAND_FUNC(NasdaqActivesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(NYSEActivesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(OTCBBActivesSubscription);
DECL_CSUB_GET_COMMAND_FUNC(OptionActivesSubscription);
/* GetCommand generic method (cast to StreamerSubscription_C*) */
DECL_CSUB_GET_COMMAND_FUNC(StreamingSubscription);
#undef DECL_CSUB_GET_COMMAND_FUNC


#define DECL_CSUB_GET_SYMBOLS_FUNC(name) \
static inline int \
name##_GetSymbols( name##_C *psub, char***buffers, size_t *n) \
{ return SubscriptionBySymbolBase_GetSymbols_ABI( \
    (StreamingSubscription_C*)psub, buffers, n, 0 ); }

/* GetSymbols methods for each SubscriptionBySymbolBase descendant*/
/* GetCommand methods for each sub type */
DECL_CSUB_GET_SYMBOLS_FUNC(QuotesSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(OptionsSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(LevelOneForexSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(NewsHeadlineSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(ChartEquitySubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(ChartFuturesSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(ChartOptionsSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(TimesaleFuturesSubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(TimesaleEquitySubscription);
DECL_CSUB_GET_SYMBOLS_FUNC(TimesaleOptionsSubscription);


#define DECL_CSUB_GET_FIELDS_FUNC(name) \
static inline int \
name##_GetFields(name##_C *psub, name##Field **fields, size_t *n) \
{ return name##_GetFields_ABI(psub, (int**)fields, n, 0); }

#define DECL_CSUB_GET_FIELDS_BASE_FUNC(name, base) \
static inline int \
name##_GetFields(name##_C *psub, base##Field **fields, size_t *n) \
{ return base##Base_GetFields_ABI((StreamingSubscription_C*)psub, \
                                   (int**)fields, n, 0); }

/* GetFields methods in derived */
DECL_CSUB_GET_FIELDS_FUNC(QuotesSubscription);
DECL_CSUB_GET_FIELDS_FUNC(OptionsSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneFuturesSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneForexSubscription);
DECL_CSUB_GET_FIELDS_FUNC(LevelOneFuturesOptionsSubscription);
DECL_CSUB_GET_FIELDS_FUNC(NewsHeadlineSubscription);
DECL_CSUB_GET_FIELDS_FUNC(ChartEquitySubscription);
#undef DECL_CSUB_GET_FIELDS_FUNC

/* GeFields methods in base */
DECL_CSUB_GET_FIELDS_BASE_FUNC(ChartFuturesSubscription, ChartSubscription);
DECL_CSUB_GET_FIELDS_BASE_FUNC(ChartOptionsSubscription, ChartSubscription);
DECL_CSUB_GET_FIELDS_BASE_FUNC(TimesaleFuturesSubscription, TimesaleSubscription);
DECL_CSUB_GET_FIELDS_BASE_FUNC(TimesaleEquitySubscription, TimesaleSubscription);
DECL_CSUB_GET_FIELDS_BASE_FUNC(TimesaleOptionsSubscription, TimesaleSubscription);
#undef DECL_CSUB_GET_FIELDS_BASE_FUNC

#define DECL_CSUB_GET_DURATION(name, base) \
static inline int \
name##_GetDuration(name##_C *psub, DurationType *duration) \
{ return base##Base_GetDuration_ABI( \
    (StreamingSubscription_C*)psub, (int*)duration, 0); }

/* GetDuration in base */
DECL_CSUB_GET_DURATION(NasdaqActivesSubscription, ActivesSubscription)
DECL_CSUB_GET_DURATION(NYSEActivesSubscription, ActivesSubscription)
DECL_CSUB_GET_DURATION(OTCBBActivesSubscription, ActivesSubscription)
DECL_CSUB_GET_DURATION(OptionActivesSubscription, ActivesSubscription)
#undef DECL_CSUB_GET_DURATION

/* GetVenue */
static inline int
OptionActivesSubscription_GetVenue(OptionActivesSubscription_C *psub,
                                   VenueType *venue)
{ return OptionActivesSubscription_GetVenue_ABI(psub, (int*)venue, 0); }


#else
/* C++ Interface */

namespace tdma{


struct StreamerCredentials{
    std::string user_id;
    std::string token;
    std::string company;
    std::string segment;
    std::string cd_domain;
    std::string user_group;
    std::string access_level;
    bool authorized;
    long long timestamp;
    std::string app_id;
    std::string acl;
};

struct StreamerInfo{
    StreamerCredentials credentials;
    std::string credentials_encoded;
    std::string url;
    std::string primary_acct_id;

    void
    encode_credentials();
};


class StreamingSubscription{
public:
    typedef StreamingSubscription_C CType;

private:
    std::shared_ptr<CType> _csub;
    friend class StreamingSession;

protected:
    template<typename CTy=CType>
    CTy*
    csub() const
    { return reinterpret_cast<CTy*>( const_cast<CType*>(_csub.get()) ); }

    template<typename CTy, typename F, typename... Args>
    StreamingSubscription(
            CTy _,
            F create_func,
            typename std::enable_if<IsValidCProxy<CTy, CType>::value>::type *v,
            Args... args )
        :
            _csub( new StreamingSubscription_C{0,0},
                   CProxyDestroyer<CType>(StreamingSubscription_Destroy_ABI) )
        {
            /* IF WE THROW BEFORE HERE WE MAY LEAK IN DERIVED */
            if( create_func )
                create_func(args..., csub<CTy>(), 1);
        }


    template<typename CTy >
    StreamingSubscription(
            CTy _,
            typename std::enable_if<IsValidCProxy<CTy, CType>::value>::type*v
                = nullptr )
        :
            _csub( new StreamingSubscription_C{0,0},
                   CProxyDestroyer<CType>(StreamingSubscription_Destroy_ABI) )
        {
        }


public:
    StreamerServiceType
    get_service() const
    {
        int ss;
        StreamingSubscription_GetService_ABI(_csub.get(), &ss, 1);
        return static_cast<StreamerServiceType>(ss);
    }

    std::string
    get_command() const
    { return str_from_abi(StreamingSubscription_GetCommand_ABI, _csub.get()); }
};


class SubscriptionBySymbolBase
        : public StreamingSubscription {
protected:
    template<typename FieldTy, typename CTy>
    std::set<FieldTy>
    fields_from_abi( int(*abicall)(CTy*, int**, size_t*,int) ) const
    { return enums_from_abi<FieldTy>(abicall, csub<CTy>()); }

    template<typename CTy, typename F, typename FTy>
    SubscriptionBySymbolBase( CTy _,
                                 F create_func,
                                 const std::set<std::string>& symbols,
                                 const std::set<FTy>& fields )
        :
            StreamingSubscription(_)
        {
            const char** s = nullptr;
            int *i = nullptr;
            try{
                s = set_to_new_cstrs(symbols);
                i = set_to_new_int_array(fields);
                create_func(s, symbols.size(), i, fields.size(), csub<CTy>(), 1);
                delete[] s;
                delete[] i;
            }catch(...){
                if( s ) delete[] s;
                if( i ) delete[] i;
                throw;
            }
        }

public:
    std::set<std::string>
    get_symbols() const
    { return set_of_strs_from_abi(SubscriptionBySymbolBase_GetSymbols_ABI, csub()); }
};


class QuotesSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = QuotesSubscriptionField;
    typedef QuotesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::QUOTE;

    QuotesSubscription( const std::set<std::string>& symbols,
                          const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( QuotesSubscription_C{},
                                      QuotesSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_QUOTES );
        }

    std::set<FieldType>
    get_fields() const
    { return fields_from_abi<FieldType>(QuotesSubscription_GetFields_ABI); }

};


class OptionsSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = OptionsSubscriptionField;
    typedef OptionsSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::OPTION;

    OptionsSubscription( const std::set<std::string>& symbols,
                         const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( OptionsSubscription_C{},
                                      OptionsSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_OPTIONS);
        }

    std::set<FieldType>
    get_fields() const
    { return fields_from_abi<FieldType>(OptionsSubscription_GetFields_ABI); }
};


class LevelOneFuturesSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = LevelOneFuturesSubscriptionField;
    typedef LevelOneFuturesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::LEVELONE_FUTURES;

    LevelOneFuturesSubscription( const std::set<std::string>& symbols,
                                    const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( LevelOneFuturesSubscription_C{},
                                      LevelOneFuturesSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_LEVEL_ONE_FUTURES);
        }

    std::set<FieldType>
    get_fields() const
    {
        return fields_from_abi<FieldType>(
            LevelOneFuturesSubscription_GetFields_ABI
            );
    }
};


class LevelOneForexSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = LevelOneForexSubscriptionField;
    typedef LevelOneForexSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::LEVELONE_FOREX;

    LevelOneForexSubscription( const std::set<std::string>& symbols,
                                  const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( LevelOneForexSubscription_C{},
                                      LevelOneForexSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_LEVEL_ONE_FOREX);
        }

    std::set<FieldType>
    get_fields() const
    {
        return fields_from_abi<FieldType>(
            LevelOneForexSubscription_GetFields_ABI
            );
    }
};


class LevelOneFuturesOptionsSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = LevelOneFuturesOptionsSubscriptionField;
    typedef LevelOneFuturesOptionsSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::LEVELONE_FUTURES_OPTIONS;

    LevelOneFuturesOptionsSubscription( const std::set<std::string>& symbols,
                                            const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( LevelOneFuturesOptionsSubscription_C{},
                                      LevelOneFuturesOptionsSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_LEVEL_ONE_FUTURES_OPTIONS);
        }

    std::set<FieldType>
    get_fields() const
    {
        return fields_from_abi<FieldType>(
            LevelOneFuturesOptionsSubscription_GetFields_ABI
            );
    }
};


class NewsHeadlineSubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = NewsHeadlineSubscriptionField;
    typedef NewsHeadlineSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::NEWS_HEADLINE;

    NewsHeadlineSubscription( const std::set<std::string>& symbols,
                                 const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( NewsHeadlineSubscription_C{},
                                      NewsHeadlineSubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_NEWS_HEADLINE );
        }

    std::set<FieldType>
    get_fields() const
    { return fields_from_abi<FieldType>(NewsHeadlineSubscription_GetFields_ABI); }
};


// TODO implement ADD command
class ChartEquitySubscription
        : public SubscriptionBySymbolBase {
public:
    using FieldType = ChartEquitySubscriptionField;
    typedef ChartEquitySubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::CHART_EQUITY;

    ChartEquitySubscription( const std::set<std::string>& symbols,
                             const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( ChartEquitySubscription_C{},
                                      ChartEquitySubscription_Create_ABI,
                                      symbols,
                                      fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_CHART_EQUITY );
        }

    std::set<FieldType>
    get_fields() const
    { return fields_from_abi<FieldType>(ChartEquitySubscription_GetFields_ABI); }
};


class ChartSubscriptionBase
        : public SubscriptionBySymbolBase {
public:
    using FieldType = ChartSubscriptionField;

protected:
    template<typename CTy, typename F>
    ChartSubscriptionBase( CTy _,
                              F func,
                              const std::set<std::string>& symbols,
                              const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( _, func, symbols, fields )
        {
        }

public:
    std::set<FieldType>
    get_fields() const
    { return fields_from_abi<FieldType>(ChartSubscriptionBase_GetFields_ABI); }
};


/*
 * NOT WORKING - EUR/USD repsonse: error 22, msg 'Bad command formatting'
 *
class ChartForexSubscription
        : public ChartSubscriptionBase {   
public:
    typedef ChartFuturesSubscription_C CType;


    ChartForexSubscription( const std::set<std::string>& symbols,
                               const std::set<FieldType>& fields )
        : ChartSubscriptionBase( StreamerServiceType::CHART_FOREX,
                                 symbols, fields )
    {}
};
*/

class ChartFuturesSubscription
        : public ChartSubscriptionBase {
public:
    typedef ChartFuturesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::CHART_FUTURES;

    ChartFuturesSubscription( const std::set<std::string>& symbols,
                                 const std::set<FieldType>& fields )
        :
            ChartSubscriptionBase( ChartFuturesSubscription_C{},
                                   ChartFuturesSubscription_Create_ABI,
                                   symbols, fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_CHART_FUTURES );
        }
};


class ChartOptionsSubscription
        : public ChartSubscriptionBase {
public:
    typedef ChartOptionsSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::CHART_OPTIONS;

    ChartOptionsSubscription( const std::set<std::string>& symbols,
                                 const std::set<FieldType>& fields )
        :
            ChartSubscriptionBase( ChartOptionsSubscription_C{},
                                   ChartOptionsSubscription_Create_ABI,
                                   symbols, fields )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_CHART_OPTIONS );
        }
};


class TimesaleSubscriptionBase
        : public SubscriptionBySymbolBase {
public:
    using FieldType = TimesaleSubscriptionField;

protected:
    template<typename CTy, typename F>
    TimesaleSubscriptionBase( CTy _,
                                 F func,
                                 const std::set<std::string>& symbols,
                                 const std::set<FieldType>& fields )
        :
            SubscriptionBySymbolBase( _, func, symbols, fields )
        {
        }

public:
    std::set<FieldType>
    get_fields() const
    {
        return fields_from_abi<FieldType>(
            TimesaleSubscriptionBase_GetFields_ABI
            );
    }
};


class TimesaleEquitySubscription
        : public TimesaleSubscriptionBase {
public:
    typedef TimesaleEquitySubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::TIMESALE_EQUITY;

    TimesaleEquitySubscription( const std::set<std::string>& symbols,
                                   const std::set<FieldType>& fields )
        :
            TimesaleSubscriptionBase( TimesaleEquitySubscription_C{},
                                      TimesaleEquitySubscription_Create_ABI,
                                     symbols, fields)
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_TIMESALE_EQUITY );
        }
};

/*
 *  * NOT WORKING - EUR/USD repsonse: error 22, msg 'Bad command formatting'
class TimesaleForexSubscription
        : public TimesaleSubscriptionBase {
public:
    typedef TimesaleForexSubscription_C CType;

    TimesaleForexSubscription( const std::set<std::string>& symbols,
                               const std::set<FieldType>& fields )
        : TimesaleSubscriptionBase( StreamerServiceType::TIMESALE_FOREX,
                                    symbols, fields)
    {}
};
*/

class TimesaleFuturesSubscription
        : public TimesaleSubscriptionBase {
public:
    typedef TimesaleFuturesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::TIMESALE_FUTURES;

    TimesaleFuturesSubscription( const std::set<std::string>& symbols,
                                   const std::set<FieldType>& fields )
        :
            TimesaleSubscriptionBase( TimesaleFuturesSubscription_C{},
                                      TimesaleFuturesSubscription_Create_ABI,
                                      symbols, fields)
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_TIMESALE_FUTURES );
        }
};

class TimesaleOptionsSubscription
        : public TimesaleSubscriptionBase {
public:
    typedef TimesaleOptionsSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::TIMESALE_OPTIONS;

    TimesaleOptionsSubscription( const std::set<std::string>& symbols,
                                    const std::set<FieldType>& fields )
        :
            TimesaleSubscriptionBase( TimesaleOptionsSubscription_C{},
                                      TimesaleOptionsSubscription_Create_ABI,
                                      symbols, fields)
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_TIMESALE_OPTIONS );
        }
};


class ActivesSubscriptionBase
        : public StreamingSubscription {
protected:
    template<typename CTy, typename F, typename... Args>
    ActivesSubscriptionBase( CTy _, F func, Args... args)
        :
            StreamingSubscription(_, func, nullptr, args...)
        {
        }

public:
    DurationType
    get_duration() const
    {
        int d;
        ActivesSubscriptionBase_GetDuration_ABI(csub(), &d, 1);
        return static_cast<DurationType>(d);
    }

};


class NasdaqActivesSubscription
        : public ActivesSubscriptionBase {
public:
    typedef NasdaqActivesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::ACTIVES_NASDAQ;

    NasdaqActivesSubscription(DurationType duration)
        :
            ActivesSubscriptionBase( NasdaqActivesSubscription_C{},
                                     NasdaqActivesSubscription_Create_ABI,
                                     static_cast<int>(duration) )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_ACTIVES_NASDAQ );
        }
};

class NYSEActivesSubscription
        : public ActivesSubscriptionBase {
public:
    typedef NYSEActivesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::ACTIVES_NYSE;

    NYSEActivesSubscription(DurationType duration)
        :
            ActivesSubscriptionBase( NYSEActivesSubscription_C{},
                                     NYSEActivesSubscription_Create_ABI,
                                     static_cast<int>(duration) )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_ACTIVES_NYSE );
        }
};

class OTCBBActivesSubscription
        : public ActivesSubscriptionBase {
public:
    typedef OTCBBActivesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::ACTIVES_OTCBB;

    OTCBBActivesSubscription(DurationType duration)
        :
            ActivesSubscriptionBase( OTCBBActivesSubscription_C{},
                                     OTCBBActivesSubscription_Create_ABI,
                                     static_cast<int>(duration) )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_ACTIVES_OTCBB );
        }
};

class OptionActivesSubscription
        : public ActivesSubscriptionBase {
public:
    typedef OptionActivesSubscription_C CType;

    static const StreamerServiceType STREAMER_SERVICE_TYPE =
        StreamerServiceType::ACTIVES_OPTIONS;

    OptionActivesSubscription(VenueType venue, DurationType duration)
        :
            ActivesSubscriptionBase( OptionActivesSubscription_C{},
                                     OptionActivesSubscription_Create_ABI,
                                     static_cast<int>(venue),
                                     static_cast<int>(duration) )
        {
            assert( csub<>()->type_id == TYPE_ID_SUB_ACTIVES_OPTION );
        }

    VenueType
    get_venue() const
    {
        int v;
        OptionActivesSubscription_GetVenue_ABI(csub<CType>(), &v, 1);
        return static_cast<VenueType>(v);
    }

};

} /* tdma */

#endif /* __cplusplus */

#define STREAMING_VERSION "1.0"
#define STREAMING_MIN_TIMEOUT 1000
#define STREAMING_MIN_LISTENING_TIMEOUT 10000
#define STREAMING_LOGOUT_TIMEOUT 1000
#define STREAMING_DEF_CONNECT_TIMEOUT 3000
#define STREAMING_DEF_LISTENING_TIMEOUT 30000
#define STREAMING_DEF_SUBSCRIBE_TIMEOUT 1500
#define STREAMING_MAX_SUBSCRIPTIONS 50

static const int TYPE_ID_STREAMING_SESSION = 100;


typedef void(*streaming_cb_ty)(int, int, unsigned long long, const char*);

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_Create_ABI( struct Credentials *pcreds,
                                streaming_cb_ty callback,
                                unsigned long connect_timeout,
                                unsigned long listening_timeout,
                                unsigned long subscribe_timeout,
                                StreamingSession_C *psession,
                                int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_Destroy_ABI( StreamingSession_C *psession,
                                 int allow_exceptions );



EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_Start_ABI( StreamingSession_C *psession,
                               StreamingSubscription_C **subs,
                               size_t nsubs,
                               int *results_buffer,
                               int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_AddSubscriptions_ABI( StreamingSession_C *psession,
                                           StreamingSubscription_C **subs,
                                           size_t nsubs,
                                           int *results_buffer,
                                           int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_Stop_ABI( StreamingSession_C *psession,
                               int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_IsActive_ABI( StreamingSession_C *psession,
                                  int *is_active,
                                  int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_SetQOS_ABI( StreamingSession_C *psession,
                                int qos,
                                int *result,
                                int allow_exceptions );

EXTERN_C_SPEC_ DLL_SPEC_ int
StreamingSession_GetQOS_ABI( StreamingSession_C *psession,
                                int *qos,
                                int allow_exceptions );

#ifndef __cplusplus

/* C Interface */

static inline int
StreamingSession_Create( struct Credentials *pcreds,
                        streaming_cb_ty callback,
                        StreamingSession_C *psession )
{
    return StreamingSession_Create_ABI(pcreds, callback,
                                       STREAMING_DEF_CONNECT_TIMEOUT,
                                       STREAMING_DEF_LISTENING_TIMEOUT,
                                       STREAMING_DEF_SUBSCRIBE_TIMEOUT,
                                       psession, 0);
}

static inline int
StreamingSession_CreateEx( struct Credentials *pcreds,
                            streaming_cb_ty callback,
                            unsigned long connect_timeout,
                            unsigned long listening_timeout,
                            unsigned long subscribe_timeout,
                            StreamingSession_C *psession )
{
    return StreamingSession_Create_ABI(pcreds, callback,
                                       connect_timeout, listening_timeout,
                                       subscribe_timeout, psession, 0);
}

static inline int
StreamingSession_Destroy( StreamingSession_C *psession )
{ return StreamingSession_Destroy_ABI(psession, 0); }


static inline int
StreamingSession_Start( StreamingSession_C *psession,
                        StreamingSubscription_C **subs,
                        size_t nsubs,
                        int *results_buffer)
{ return StreamingSession_Start_ABI(psession, subs, nsubs, results_buffer, 0); }


static inline int
StreamingSession_AddSubscriptions( StreamingSession_C *psession,
                                   StreamingSubscription_C **subs,
                                   size_t nsubs,
                                   int *results_buffer )
{ return StreamingSession_AddSubscriptions_ABI(psession, subs, nsubs,
                                               results_buffer, 0); }

static inline int
StreamingSession_Stop( StreamingSession_C *psession )
{ return StreamingSession_Stop_ABI(psession, 0); }

static inline int
StreamingSession_IsActive( StreamingSession_C *psession, int *is_active )
{ return StreamingSession_IsActive_ABI( psession, is_active, 0 ); }

static inline int
StreamingSession_SetQOS(StreamingSession_C *psession, QOSType qos, int *result)
{ return StreamingSession_SetQOS_ABI(psession, (int)qos, result, 0); }

static inline int
StreamingSession_GetQOS( StreamingSession_C *psession, QOSType *qos)
{ return StreamingSession_GetQOS_ABI(psession, (int*)qos, 0); }

#else

/* C++ Interface */

namespace tdma{

class DLL_SPEC_ StreamingSession{
public:
    static const std::string VERSION; // = "1.0"
    static const std::chrono::milliseconds MIN_TIMEOUT; // 1000;
    static const std::chrono::milliseconds MIN_LISTENING_TIMEOUT; // 10000;
    static const std::chrono::milliseconds LOGOUT_TIMEOUT; // 1000
    static const std::chrono::milliseconds DEF_CONNECT_TIMEOUT; // 3000
    static const std::chrono::milliseconds DEF_LISTENING_TIMEOUT; // 30000
    static const std::chrono::milliseconds DEF_SUBSCRIBE_TIMEOUT; // 1500
    static const int MAX_SUBSCRIPTIONS = STREAMING_MAX_SUBSCRIPTIONS; // 50

    typedef StreamingSession_C CType;

private:
    std::unique_ptr<CType, CProxyDestroyer<CType>> _obj;

    StreamingSession()
        :
            _obj( new CType{0,0,0},
                  CProxyDestroyer<CType>(StreamingSession_Destroy_ABI) )
        {}

    std::deque<bool>
    _call_abi_with_subs(
        std::function<int(CType*, StreamingSubscription_C**,
                          size_t, int*, int)> abicall,
        const std::vector<StreamingSubscription>& subscriptions
        )
    {
        std::deque<bool> cpp_results;
        StreamingSubscription_C **buffer = nullptr;
        int *results = nullptr;
        size_t sz = subscriptions.size();
        try{
            buffer = new StreamingSubscription_C*[sz];
            std::transform(subscriptions.begin(), subscriptions.end(), buffer,
                           [](const StreamingSubscription& s){ return s.csub(); });

            results = new int[sz];
            abicall(_obj.get(), buffer, sz, results, 1);
            while( sz-- ){
                cpp_results.push_front(static_cast<bool>(results[sz]));
            }
            delete[] buffer;
            delete[] results;
        }catch(...){
            if( buffer ) delete[] buffer;
            if( results ) delete[] results;
            throw;
        }
        return cpp_results;
    }

public:
    static std::shared_ptr<StreamingSession>
    Create( Credentials& creds,
             streaming_cb_ty callback,
             std::chrono::milliseconds connect_timeout=DEF_CONNECT_TIMEOUT,
             std::chrono::milliseconds listening_timeout=DEF_LISTENING_TIMEOUT,
             std::chrono::milliseconds subscribe_timeout=DEF_SUBSCRIBE_TIMEOUT
             )
    {
        StreamingSession *ss = nullptr;
        try{
            ss = new StreamingSession;
            StreamingSession_Create_ABI( &creds, callback,
                                         connect_timeout.count(),
                                         listening_timeout.count(),
                                         subscribe_timeout.count(),
                                         ss->_obj.get(), 1 );
            assert( ss->_obj.get()->type_id == TYPE_ID_STREAMING_SESSION );
        }catch(...){
            if( ss ) delete ss;
            throw;
        }
        return std::shared_ptr<StreamingSession>(ss);
    }

    StreamingSession( const StreamingSession& ) = delete;

    StreamingSession&
    operator=( const StreamingSession& ) = delete;

    std::deque<bool> // success/fails in the order passed
    start(const std::vector<StreamingSubscription>& subscriptions)
    { return _call_abi_with_subs(StreamingSession_Start_ABI,subscriptions); }

    bool 
    start(const StreamingSubscription& subscription)
    { return start(std::vector<StreamingSubscription>{subscription})[0]; }

    void
    stop()
    { StreamingSession_Stop_ABI(_obj.get(), 1); }

    bool
    is_active() const
    {
        int active;
        StreamingSession_IsActive_ABI(_obj.get(), &active, 1);
        return static_cast<bool>(active);
    }

    std::deque<bool> // success/fails in the order passed
    add_subscriptions(const std::vector<StreamingSubscription>& subscriptions)
    { return _call_abi_with_subs(StreamingSession_AddSubscriptions_ABI,
                                 subscriptions); }

    bool 
    add_subscription(const StreamingSubscription& subscription)
    { return add_subscriptions(std::vector<StreamingSubscription>{subscription})[0]; }


    QOSType
    get_qos() const
    {
        int q;
        StreamingSession_GetQOS_ABI(_obj.get(), &q, 1);
        return static_cast<QOSType>(q);
    }

    bool
    set_qos(const QOSType& qos)
    {
        int result;
        StreamingSession_SetQOS_ABI(_obj.get(), static_cast<int>(qos), &result, 1);
        return static_cast<bool>(result);
    }
};


} /* amtd */

#endif /* __cplusplus */

#endif // TDMA_API_STREAMING_H
