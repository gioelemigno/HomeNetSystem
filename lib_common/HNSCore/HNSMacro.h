
 /*****************************************************************************
 *
 * FILENAME :    	    				HNSMacro.h                			DESIGN REF: 00 0X
 *
 * DESCRIPTION :
 *       This file contains the value of macros used in other file.
 *			 There are these types of macros:
 *					-	type of trasmission
 *					-	flag to specif error
 *
 * PUBLIC FUNCTIONS :
 *			 none
 *
 * NOTES :
 *
 *
 * AUTHOR :    Gioele Migno      START DATE :    04 july 2017
 *       (gioele.migno@gmail.com)
 *
 * CHANGES :
 *
 * REF NO   VERSION       DATE        WHO      DETAIL
 *   00     v.0.4.2      04July17      GM       File creation
 *
 *****************************************************************************/

 /******************************************************************************
 *
 *    LICENSE:  GNU General Public License v3.0
 *
 ******************************************************************************/

#ifndef HNSMACRO_H_
#define HNSMACRO_H_


//	TYPES OF TRASMISSION	:		[0	-	249] [250 - 254]
/*****************************************************************************/
// Special type :
	#define ACK 										253
	#define ADDRESS_ASSIGNMENT			252
	#define SEND_MODEL							251
	#define SYNC										250

//	Read:	(they are used by master)
	#define READ_ALL 								249
	#define READ_1	                248
	#define READ_2	                247
	#define READ_3	                246
	#define READ_4	                245
	#define READ_5	                244
	#define READ_6	                243
	#define READ_7									242
	#define READ_8									241
	#define READ_9									240
	#define READ_10									239
	#define READ_11									238
	#define READ_12									237
  #define READ_13									236
	#define READ_14									235
	#define READ_15									234
	#define READ_16									233
	#define READ_17									232
	#define READ_18									231
	#define READ_19									230
	#define READ_20									229
	#define READ_21									228
	#define READ_22									227
	#define READ_23									226
	#define READ_24									225
	#define READ_25									224
	#define READ_26									223
	#define READ_27									222
	#define READ_28									221
	#define READ_29									220
	#define READ_30									219
	#define READ_31									218

//	Set:	(they are used by master)
	#define SET 										217
	#define SET_1										216
	#define SET_2										215
	#define SET_3										214
	#define SET_4										213
	#define SET_5										212
	#define SET_6										211
	#define SET_7										210
	#define SET_8										209
	#define SET_9										208
	#define SET_10									207
	#define SET_11									206
	#define SET_12									205
	#define SET_13									204
	#define SET_14									203
	#define SET_15									202
	#define SET_16									201
	#define SET_17									200
	#define SET_18									199
	#define SET_19									198
	#define SET_20									197
	#define SET_21									196
	#define SET_22									195
	#define SET_23									194
	#define SET_24									193
	#define SET_25									192
	#define SET_26									191
	#define SET_27									190
	#define SET_28									189
	#define SET_29									188
	#define SET_30									187
	#define SET_31									186

	//	Other types	:
	#define RESPONSE _0							185			//it used by slave to give a
																					//information to master
	#define RESPONSE_1	184
	#define RESPONSE_2	183
	#define RESPONSE_3	182
	#define RESPONSE_4	181
	#define RESPONSE_5	180
	#define RESPONSE_6	179
	#define RESPONSE_7	178
	#define RESPONSE_8	177
	#define RESPONSE_9	176
	#define RESPONSE_10	175
	#define RESPONSE_11	174
	#define RESPONSE_12	173
	#define RESPONSE_13	172
	#define RESPONSE_14	171
	#define RESPONSE_15	170
	#define RESPONSE_16	169
	#define RESPONSE_17	168
	#define RESPONSE_18	167
	#define RESPONSE_19	166
	#define RESPONSE_20	165
	#define RESPONSE_21	164
	#define RESPONSE_22	163
	#define RESPONSE_23	162
	#define RESPONSE_24	161
	#define RESPONSE_25	160
	#define RESPONSE_26	159
	#define RESPONSE_27	158
	#define RESPONSE_28	157
	#define RESPONSE_29	156
	#define RESPONSE_30	155
	#define RESPONSE_31	154




	#define CONFIRM 								153			//it used by slave to
																					//confirm last setup
/*****************************************************************************/






//DATA 8	:
/*****************************************************************************/
//It specific the thing or indicate a accurate status
//	Thing	:
//data that specific the thing to act on it when the type of trasmission
//is read or set
	#define THING_0									249
	#define THING_1									248
	#define THING_2									247
	#define THING_3									246
	#define THING_4									245
	#define THING_5									244
	#define THING_6									243
	#define THING_7									242
	#define THING_8									241
	#define THING_9									240
	#define THING_10								239
	#define THING_11								238
	#define THING_12								237
	#define THING_13								236
	#define THING_14								235
	#define THING_15								234
	#define THING_16								233
	#define THING_17								232
	#define THING_18								231
	#define THING_19								230
	#define THING_20								229
	#define THING_21								228
	#define THING_22								227
	#define THING_23								226
	#define THING_24								225
	#define THING_25								224
	#define THING_26								223
	#define THING_27								222
	#define THING_28								221
	#define THING_29								220
	#define THING_30								219
	#define THING_31								218

//	Status  :
	#define STATUS_0								217
	#define STATUS_1								216
	#define STATUS_2								215
	#define STATUS_3								214
	#define STATUS_4								213
	#define STATUS_5								212
	#define STATUS_6								211
	#define STATUS_7								210
	#define STATUS_8								209
	#define STATUS_9								208
	#define STATUS_10								207
	#define STATUS_11								206
	#define STATUS_12								205
	#define STATUS_13								204
	#define STATUS_14								203
	#define STATUS_15								202
	#define STATUS_16								201
	#define STATUS_17								200
	#define STATUS_18								199
	#define STATUS_19								198
	#define STATUS_20								197
	#define STATUS_21								196
	#define STATUS_22								195
	#define STATUS_23								194
	#define STATUS_24								193
	#define STATUS_25								192
	#define STATUS_26								191
	#define STATUS_27								190
	#define STATUS_28								189
	#define STATUS_29								188
	#define STATUS_30								187
	#define STATUS_31								186
/*****************************************************************************/

	#define ACK_1										185
	#define ACK_2										184
	#define ACK_3										183
	#define ACK_4										182
	#define ACK_5										181


typedef enum HNSModel {

	HNSModel_HNSHub = 1,
	HNSModel_HNSTemperatureSensor,
	HNSModel_HNSLight,
	HNSModel_HNSTemperatureHumiditySensor,
	HNSModel_HNSSocketCurrentSensor

}HNSModel;

#endif /* HNSMACRO_H_ */
