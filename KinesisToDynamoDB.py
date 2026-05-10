import base64
import json
import boto3
import urllib.request
import urllib.parse
from decimal import Decimal

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('') #ใส่ชื่อ Tables DynamoDB ที่สร้างขึ้น

CHANNEL_ACCESS_TOKEN = '' #ใส่ Channel access token ของ Bot ใน LINE Developer
USER_ID = '' #ใส่ USER_ID ใน LINE Developer
NOISE_THRESHOLD = 40.0  

def send_line_bot(message):
    url = 'https://api.line.me/v2/bot/message/push'
    headers = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {CHANNEL_ACCESS_TOKEN}'
    }

    data = {
        "to": USER_ID,
        "messages": [{"type": "text", "text": message}]
    }
    
    payload = json.dumps(data).encode('utf-8')
    req = urllib.request.Request(url, headers=headers, data=payload)
    
    try:
        urllib.request.urlopen(req)
        print("✅ ส่ง LINE Bot สำเร็จ!")
    except Exception as e:
        print(f"❌ ส่ง LINE Bot พลาด: {e}")

def lambda_handler(event, context):
    for record in event['Records']:
        payload = base64.b64decode(record['kinesis']['data']).decode('utf-8')
        
        data = json.loads(payload, parse_float=Decimal)
        
        print(f"ได้รับข้อมูล: {data}")
        
        table.put_item(
            Item={
                'deviceId': data['deviceId'],
                'timestamp': data['timestamp'],
                'decibel': data['decibel'],
                'latitude': data['latitude'],
                'longitude': data['longitude']
            }
        )
        if data['decibel'] >= NOISE_THRESHOLD:
            alert_msg = f"⚠️ แจ้งเตือนเสียงดังผิดปกติ!\n📍 อุปกรณ์: {data['deviceId']} พิกัด:{data['latitude']},{data['longitude']}\n🔊 ความดัง: {data['decibel']} dB\n🕒 เวลา: {data['timestamp']}"
            send_line_bot(alert_msg)
    return {
        'statusCode': 200,
        'body': json.dumps('บันทึกข้อมูลเรียบร้อย!')
    }
