# TUNOTIFICATION
# INTRODUCE
TUNotification คือระบบตรวจจับและแจ้งเตือนมลภาวะทางเสียงแบบ Real-time ที่พัฒนาขึ้นโดยใช้เทคโนโลยี IoT และ Cloud Computing เพื่อช่วยตรวจสอบระดับเสียงรบกวนภายในพื้นที่มหาวิทยาลัยธรรมศาสตร์ ระบบทำงานโดยรับข้อมูลเสียงจากอุปกรณ์ IoT เช่น ESP32 และเซนเซอร์ INMP441 จากนั้นส่งข้อมูลเข้าสู่ระบบ Cloud บน AWS เพื่อประมวลผล วิเคราะห์ และแสดงตำแหน่งของแหล่งกำเนิดเสียงผ่าน Web Application ที่เชื่อมต่อกับ Google Maps API เมื่อระบบตรวจพบระดับเสียงที่สูงผิดปกติ จะสามารถแจ้งเตือนไปยังผู้ใช้งานหรือเจ้าหน้าที่ที่เกี่ยวข้องได้แบบ Real-time ช่วยให้สามารถเข้าตรวจสอบและจัดการปัญหาได้รวดเร็วขึ้น โปรเจคนี้ถูกพัฒนาขึ้นเพื่อเพิ่มประสิทธิภาพในการจัดการปัญหามลภาวะทางเสียง ลดระยะเวลาในการค้นหาต้นตอของปัญหา และช่วยสร้างสภาพแวดล้อมที่เหมาะสมต่อการเรียนและการใช้งานภายในมหาวิทยาลัยธรรมศาสตร์

# Pain Points
ปัญหามลภาวะทางเสียงภายในมหาวิทยาลัยหรือพื้นที่สาธารณะ มักประสบปัญหาในการตรวจสอบและจัดการ ดังนี้
- ผู้ใช้งานและเจ้าหน้าที่ไม่สามารถทราบตำแหน่งของแหล่งกำเนิดเสียงได้อย่างชัดเจน <br>
- การตรวจสอบและจัดการปัญหามักล่าช้า เนื่องจากต้องอาศัยการแจ้งเหตุแบบ Manual <br>
- ไม่มีระบบติดตามและแจ้งเตือนแบบ Real-time ทำให้ยากต่อการตอบสนองต่อเหตุการณ์ทันที <br>
- ขาดข้อมูลเชิงสถิติและข้อมูลตำแหน่งของเสียงรบกวนเพื่อใช้วิเคราะห์ปัญหา <br>
- การจัดการปัญหาไม่สามารถแก้ไขได้ตรงจุด เนื่องจากไม่ทราบต้นตอของเสียงอย่างแม่นยำ <br>

โปรเจคนี้จึงถูกพัฒนาขึ้นเพื่อช่วยตรวจจับ วิเคราะห์ และแจ้งเตือนปัญหามลภาวะทางเสียงแบบอัตโนมัติ โดยใช้เทคโนโลยี IoT และ Cloud Service เพื่อเพิ่มความรวดเร็วและความแม่นยำในการจัดการปัญหา

# Solution
ระบบ TU-Sound-Notification ถูกพัฒนาขึ้นเพื่อแก้ไขปัญหามลภาวะทางเสียงภายในมหาวิทยาลัย โดยใช้เทคโนโลยี IoT ร่วมกับ Cloud Services บน AWS เพื่อให้สามารถตรวจจับ วิเคราะห์ และแจ้งเตือนปัญหาเสียงรบกวนได้แบบ Real-time อุปกรณ์ IoT ที่ติดตั้งภายในพื้นที่ จะทำหน้าที่ตรวจวัดระดับเสียงผ่านเซนเซอร์ INMP441 และส่งข้อมูลเข้าสู่ระบบ Cloud ผ่าน ESP32 จากนั้นข้อมูลจะถูกประมวลผลด้วยบริการต่างๆบน AWS เพื่อจัดเก็บ วิเคราะห์ และส่งต่อไปยัง Web Application ผู้ใช้งานสามารถตรวจสอบตำแหน่งและระดับความรุนแรงของเสียงรบกวนผ่านหน้าเว็บไซต์ที่เชื่อมต่อกับ Google Maps API ได้แบบ Real-time ทำให้สามารถระบุจุดเกิดปัญหาได้อย่างแม่นยำ นอกจากนี้ ระบบยังสามารถต่อยอดไปสู่การแจ้งเตือนผ่าน LINE Notification หรือระบบแจ้งเตือนอื่นๆ เพื่อช่วยให้เจ้าหน้าที่สามารถเข้าจัดการปัญหาได้รวดเร็วขึ้น

# AWS Architecture


<br><br>
# ขั้นตอนการติดตั้ง
การติดตั้งระบบ TUNotification มีการใช้งานอุปกรณ์ IoT และบริการของ AWS โดยมีขั้นตอนดังนี้
## SECTION 1 : AWS Kinesis data stream/Iot device/Lambda function
### AWS Kinesis data stream : เตรียมสร้างท่อส่งข้อมูล
1.ค้นหา **Kinesis** ใน Search bar <br>
2.กด **Create data stream** <br>
3.ตั้งชื่อ Data stream name ว่า **SoundDataStream** <br>
4.เลือก Capacity Mode เป็น **Provisioned** <br>
5.เลือกจำนวน **Provisioned Shards** เป็น 1 <br>
6.กด **Create data stream** 

---
### IoT Device / Iot Core : เชื่อมต่อและส่งข้อมูลด้วย IoT Core
การติดตั้งอุปกรณ์ IoT และการเชื่อมต่อ IoT core สามารถดูได้ที่ https://github.com/TheepprakornRakfaen/CS262-TUNotification/blob/dad5a3872f94f256ca8c50dc4dc837bfe4db1e07/IoTDevice.md


---
### lambda function : ส่งข้อมูลไปยัง Database ด้วย Lambda function
1.เขียนโค้ดดังนี้ลงใน lambda function
```Python
    const { DynamoDBClient } = require("@aws-sdk/client-dynamodb");
    const { DynamoDBDocumentClient, PutCommand } = require("@aws-sdk/lib-dynamodb");
    
    const client = new DynamoDBClient({});
    const docClient = DynamoDBDocumentClient.from(client);
      exports.handler = async (event) => {
      console.log("Receiving data from Kinesis...");
        for (const record of event.Records) {
            try {
                // 1. Decode ข้อมูลจาก Base64 เป็น String และแปลงเป็น JSON
                const payload = Buffer.from(record.kinesis.data, 'base64').toString('ascii');
                const data = JSON.parse(payload);
              
              console.log("Decoded Data:", data);
           
              const params = {
                  TableName: "Your_DynamoDB_Table_Name", // เปลี่ยนเป็นชื่อตารางของคุณ
                  Item: {
                      "location_name": data.location_name, // Partition Key
                      "timestamp": data.timestamp || new Date().toISOString(), // Sort Key (ถ้ามี)
                      "coordinates": data.coordinates,
                      "noise_level": data.noise_level,
                      "raw_data": data
                  }
              };
              
              await docClient.send(new PutCommand(params));
              console.log("Successfully saved to DynamoDB");
  
          } catch (error) {
              console.error("Error processing record:", error);
          }
      }
      
      return { status: "success" };
    };"
```

2.ไปที่เมนู Configuration ในหน้า Lambda <br>
3.เลือกที่ชื่อว่า Permissions <br>
4.ส่วนของ Execution role, ให้กดปุ่ม Edit <br>
5.แล้วกด Save

<br><br>
## SECTION 2 : DATABASE
### DynamoDB
1. ตั้งชื่อตาราง และกำหนด Partition Key / Sort Key
- ตั้งชื่อตาราง TUNotification ให้ตารางมีเอกลักษณ์ สะดวกในการอ้างอิงจาก Lambda และ API
- Partition Key = DeviceID กระจายข้อมูลไปยังเซิร์ฟเวอร์หลายๆ ตัว (Sharding) ทำให้ระบบรองรับข้อมูลปริมาณมากได้ และค้นหาตามสถานที่ได้เร็ว
- Sort Key = timestamp เรียงลำดับข้อมูลตามเวลาได้ทันที รองรับการ Query ช่วงเวลา (เช่น "ดูข้อมูลตึกA ช่วง 9:00-10:00")

2. เลือก On-demand Capacity
- **On-demand** : ไม่ต้องคาดเดาปริมาณการใช้งานล่วงหน้า ระบบจะปรับขนาดอัตโนมัติตามปริมาณข้อมูลที่เข้ามา
Encryption
- **AWS owned key** : เข้ารหัสข้อมูลที่เก็บในฐานข้อมูลโดยอัตโนมัติ ป้องกันข้อมูลรั่วไหลหากมีผู้ไม่ได้รับอนุญาตเข้าถึง
**Attributes** อื่นๆ
- ในขั้นตอนนี้เมื่อรับข้องมูลจาก IOT เข้ามาจะทำการสร้าง Attributes อัตโนมัติ อย่าง
- - coordinates	เก็บพิกัด GPS เพื่อแสดงบนแผนที่ในเว็บ
  - noise_level	ค่าระดับเสียงที่วัดได้ ใช้เป็นข้อมูลหลักในการแจ้งเตือน
  - raw_data	เก็บข้อมูลดิบทั้งหมดจากเซนเซอร์ กรณีต้องการตรวจสอบย้อนหลังหรือ Debug
  
**สรุปจุดประสงค์หลักของ DynamoDB**
- เก็บข้อมูล Real-time	รับข้อมูลจากเซนเซอร์ที่ส่งมาตลอดเวลา
- Query เร็ว	ค้นหาข้อมูลตามสถานที่และช่วงเวลาได้ทันที
- รองรับการขยาย	ข้อมูลเพิ่มเป็นล้านแถว ระบบก็ยังทำงานได้ปกติ
- เชื่อมต่อกับ API	ให้หน้าเว็บดึงข้อมูลไปแสดงผลได้

Database Schema

<img width="1797" height="451" alt="image" src="https://github.com/user-attachments/assets/223eab3a-e429-46c5-843a-867f8acd6a6f" />




<br><br>



## SECTION 3 : WEB APPLICATION

### Lambda function : สร้าง Lambda funcion สำหรับดึงข้อมูล
1. ไปที่ AWS Console → ค้นหา Lambda <br>
2. คลิก Create function <br>
3. Author from scratch <br>
4. กรอกข้อมูลดังนี้
 - Function name: getSoundData
 - Runtime: Python 3.12
 - Architecture: x86_64
 - Permissions: เลือก Use an existing role → เลือก Labrole
5. คลิก Create function <br>
6. นำโค้ดต่อไปนี้วางลงใน function

```Python
import json
import boto3
from decimal import Decimal

dynamodb = boto3.resource('dynamodb', region_name='us-east-1')  
table = dynamodb.Table('TUNotification')  

def decimal_default(obj):
    if isinstance(obj, Decimal):
        return float(obj)
    raise TypeError

def lambda_handler(event, context):
    try:
        response = table.scan()
        items = response.get('Items', [])
        
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Content-Type': 'application/json'
            },
            'body': json.dumps(items, default=decimal_default)
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'headers': {'Access-Control-Allow-Origin': '*'},
            'body': json.dumps({'error': str(e)})
        }
```
7.หลังวาง Code แล้วคลิก Deploy (ปุ่มสีส้ม) <br>

---
### API Gateway : API สำหรับดึงข้อมูลแสดงบนหน้าเว็ป
1. AWS Console → ค้นหา **API Gateway**  <br>
2. คลิก **Create API**  <br>
3. **HTTP API** → คลิก **Build**  <br>
4. API name ว่า **sound-monitoring-api**  <br>
5. **Next** → **Next** → **Create**  <br>
6. สร้าง Route และเชื่อมกับ **Lambda**  <br>
7. Routes ด้านซ้าย → คลิก Create กรอกข้อมูลดังนี้ <br>
- Method: GET
- Path: /sound
8. Create <br>
9. Route GET /sound → คลิก **Attach integration** <br>
10. **Create and attach an integration** → เลือก **Lambda function** <br>
11. Lambda Function ชื่อ **getSoundData** → คลิก Create <br>
12. CORS ด้านซ้าย → คลิก **Configure** กรอกข้อมูลดังนี้ <br>
- Access-Control-Allow-Origin: ***** <br>
- Access-Control-Allow-Headers: *****
- Access-Control-Allow-Methods: GET, OPTIONS <br>
13. คลิก **Save** <br>
14. คลิกเมนู **Deploy** ด้านซ้าย เลือก **Stage** เป็น $default <br>
15. คลิก **Deploy** <br>
ผลลัพธ์สามารถคัดลอก URL นั้นคือ https://xxxx.execute-api.ap-southeast-1.amazonaws.com/sound ใส่ใน Web application เพื่อดึงข้อมูลได้ <br>

---
### Amplify 
(ต้นน้ำ - เขียนขั้นตอนการสร้าง Amplify และขึ้นหน้าเว็ป)

# ขั้นตอนการ Deploy หน้าเว็บไซต์ด้วย AWS Amplify

เอกสารนี้อธิบายขั้นตอนการนำไฟล์ Frontend ของโปรเจกต์ ขึ้นโฮสต์บน AWS Amplify เพื่อให้สามารถเข้าถึงหน้าเว็บผ่านอินเทอร์เน็ตได้จริง

---

## วิธีที่ 1: การ Deploy แบบ Manual (ลากวางไฟล์ - ง่ายและเร็วที่สุด)
วิธีนี้เหมาะสำหรับการทดสอบระบบอย่างรวดเร็ว โดยไม่ต้องใช้ Git

**ขั้นตอน:**
1. **เตรียมไฟล์:** นำไฟล์ทั้งหมด (`index.html`, `style.css`, `script.js`) มาบีบอัดเป็นไฟล์เดียว เช่น `website.zip` (อย่า Zip ตัวโฟลเดอร์ ให้คลุมดำที่ตัวไฟล์ทั้ง 3 แล้วกด Zip)
2. **เข้าสู่ AWS Amplify:** 
   - ล็อกอินเข้า AWS Console
   - พิมพ์ค้นหาคำว่า `Amplify` ในช่องค้นหาด้านบน แล้วคลิกเลือก AWS Amplify
3. **สร้างแอปพลิเคชัน:**
   - ในหน้าต่างด้านซ้าย เลือก All apps
   - คลิกปุ่ม Create new app (หรือ New app -> Host web app)
4. **เลือกวิธีอัปโหลด:**
   - ในหน้า Host your web app ให้เลื่อนลงมาด้านล่างสุด แล้วเลือก Deploy without Git provider (หรือ Manual deploy)
   - ตั้งชื่อแอป (App name) เช่น `IoT-Noise-Map`
   - ตั้งชื่อ Environment name เช่น `dev` หรือ `main`
5. **อัปโหลดไฟล์:**
   - ลากไฟล์ `website.zip` ที่เราเตรียมไว้มาวางในช่อง Drag and drop
   - กดปุ่ม Save and deploy
6. **เสร็จสิ้น!** รอระบบทำงานประมาณ 1-2 นาที เมื่อเสร็จแล้วคุณจะได้รับ Domain URL (เช่น `https://dev.xxxx.amplifyapp.com`) สามารถคลิกเพื่อดูหน้าเว็บได้ทันที

---

## วิธีที่ 2: การ Deploy ผ่าน GitHub (แนะนำ - อัปเดตอัตโนมัติเมื่อแก้โค้ด)
วิธีนี้เป็นมาตรฐานสากล (CI/CD) เมื่อคุณแก้ไขโค้ดแล้วกด Push ขึ้น GitHub หน้าเว็บใน Amplify จะอัปเดตตามทันที

**ขั้นตอน:**
1. **นำโค้ดขึ้น GitHub:**
   - สร้าง Repository ใหม่บน GitHub
   - อัปโหลดไฟล์โปรเจกต์ (`index.html`, `style.css`, `script.js`) ขึ้นไปบน Repository นั้น
2. **เข้าสู่ AWS Amplify:**
   - ล็อกอินเข้า AWS Console และเข้าไปที่บริการ AWS Amplify
   - คลิกปุ่ม New app -> Host web app
3. **เชื่อมต่อ GitHub:**
   - เลือก GitHub เป็น Source code provider แล้วกด Continue
   - กด Authorize เพื่ออนุญาตให้ AWS เข้าถึง GitHub ของคุณ
4. **เลือก Repository:**
   - เลือก Repository โปรเจกต์ที่คุณเพิ่งสร้าง
   - เลือก Branch (ปกติจะเป็น `main` หรือ `master`) แล้วกด Next
5. **ตั้งค่า Build Settings:**
   - เนื่องจากโปรเจกต์นี้เป็น HTML/JS ธรรมดา (ไม่ใช่ React/Vue) Amplify จะตรวจสอบเจอและสร้างการตั้งค่าให้โดยอัตโนมัติ
   - ตรวจสอบความถูกต้องแล้วกด Next
6. **ยืนยันการติดตั้ง:**
   - ตรวจสอบข้อมูลทั้งหมดแล้วกด Save and deploy
7. **เสร็จสิ้น!** 
   - ระบบจะเข้าสู่กระบวนการ Provision -> Build -> Deploy
   - เมื่อแถบสถานะเป็นสีเขียวทั้งหมด คุณสามารถคลิกที่ Domain URL ด้านซ้ายมือเพื่อเข้าใช้งานเว็บไซต์ได้เลย

---

## การอัปเดตหน้าเว็บในอนาคต

- **ถ้าใช้วิธีที่ 1 (Manual):** เมื่อมีการแก้ไขไฟล์ ให้ทำการ Zip ไฟล์ใหม่ เข้าไปที่หน้า Amplify ของโปรเจกต์นี้ แล้วลากไฟล์ Zip ใหม่ไปวางทับในช่องของ Environment เดิม
- **ถ้าใช้วิธีที่ 2 (GitHub):** เพียงแค่คุณแก้ไขโค้ดในคอมพิวเตอร์ และรันคำสั่ง `git push` ระบบ AWS Amplify จะทำการดึงโค้ดใหม่ไปอัปเดตหน้าเว็บให้อัตโนมัติในไม่กี่นาที

---

## การตั้งค่าเพิ่มเติม (สำหรับ API Gateway)
*(สำคัญสำหรับโปรเจกต์นี้)* เมื่อ Deploy หน้าเว็บสำเร็จแล้ว ให้คัดลอก Domain URL ของเว็บไซต์ ไปตั้งค่า CORS (Cross-Origin Resource Sharing) ในหน้าตั้งค่าของ AWS API Gateway ด้วย เพื่ออนุญาตให้หน้าเว็บนี้สามารถดึงข้อมูลพิกัดเสียงมาจาก API ได้โดยไม่ติดบล็อกความปลอดภัย

---


### MAP API

> แสดงผลด้วย Interactive Heatmap บนแผนที่จริง ผ่าน Leaflet.js

## ขั้นตอนการเชื่อมต่อแผนที่กับหน้าเว็บไซต์

ขั้นตอนที่ 1 HTML เชื่อม CSS และ JavaScript เข้าหากัน

**`index.html`** ทำหน้าที่เป็นจุดศูนย์กลางที่โหลดทุกอย่างเข้ามารวมกัน

```html
<head>
    <!-- 1. CSS ของ Leaflet (ต้องโหลดก่อน style.css) -->
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />

    <!-- 2. CSS ของเราเอง -->
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <!-- 3. UI ลอยเหนือแผนที่ -->
    <div class="top-ui-container"> ... </div>

    <!-- 4. พื้นที่วางแผนที่ (ต้องมี id="map" ตรงกับ script.js) -->
    <div id="map"></div>

    <!-- 5. โหลด JS ตามลำดับก่อนปิด </body> เสมอ -->
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
    <script src="https://unpkg.com/leaflet.heat/dist/leaflet-heat.js"></script>
    <script src="script.js"></script>
</body>
```

> **ลำดับ `<script>` สำคัญมาก** — `leaflet.js` ต้องมาก่อน `leaflet-heat.js` และ `script.js` เสมอ

---

ขั้นตอนที่ 2 CSS ทำให้แผนที่เต็มหน้าจอและ UI ลอยอยู่ด้านบน

CSS มีบทบาท 2 อย่างในโปรเจกต์นี้

**1) ทำให้ `#map` เต็มหน้าจอ**

```css
html, body {
    height: 100%;     /* จำเป็น — ถ้าไม่มี height: 100% บน #map จะไม่ทำงาน */
    margin: 0;
    overflow: hidden; /* ป้องกัน Scrollbar โผล่ */
}

#map {
    height: 100%;     /* รับค่าจาก body ด้านบน */
    width: 100%;
}
```

**2) ทำให้ Search Box และปุ่ม ⚙️ ลอยเหนือแผนที่**

```css
.top-ui-container {
    position: absolute;          /* ลอยออกจาก flow ปกติ */
    top: 20px;
    left: 50%;
    transform: translateX(-50%); /* จัดกึ่งกลางแนวนอน */
    z-index: 1000;               /* ต้องสูงกว่า Leaflet ที่ใช้ z-index: 400–600 */
}
```

---

ขั้นตอนที่ 3 JavaScript สร้างแผนที่และ Heatmap ลงใน `<div id="map">`

```javascript
// 1. อ่าน <div id="map"> แล้ววางแผนที่ลงไป
map = L.map('map').setView(LC2_LOCATION, 18);

// 2. โหลดกระเบื้องแผนที่จาก OpenStreetMap เป็น Background
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);

// 3. ดึงข้อมูล IoT แล้ววาด Heatmap ทับลงบนแผนที่
renderHeatmap();

// 4. วางจุดสีฟ้า "คุณอยู่ที่นี่"
L.circleMarker(LC2_LOCATION, { ... }).addTo(map);
```

---

## ภาพรวมการทำงาน (Flow)

```
เปิดหน้าเว็บ (index.html)
        │
        ├── โหลด leaflet.css   → จัด style ภายในของแผนที่
        ├── โหลด style.css     → กำหนดขนาด #map และ z-index ของ UI
        │       ├── html, body { height: 100% }   → body เต็มหน้าจอ
        │       ├── #map { height: 100% }          → แผนที่เต็มหน้าจอ
        │       └── .top-ui-container { z-index: 1000 } → UI ลอยอยู่เหนือแผนที่
        │
        ├── โหลด leaflet.js        → สร้างตัวแปร L พร้อมใช้งาน
        ├── โหลด leaflet-heat.js   → เพิ่มความสามารถ L.heatLayer()
        └── โหลด script.js         → รัน initMap()
                │
                ├── L.map('map')       → แผนที่ปรากฏใน <div id="map">
                ├── L.tileLayer()      → โหลดแผนที่ OSM เป็น Background
                ├── L.heatLayer()      → วาด Heatmap ระดับเสียง
                └── L.circleMarker()   → วางจุดตำแหน่งปัจจุบัน
```

---

## ความสัมพันธ์ระหว่าง 3 ไฟล์

| ไฟล์ | หน้าที่ | เชื่อมกับ |
|---|---|---|
| `index.html` | โครงสร้างหน้าเว็บ + จุดเชื่อมทุกอย่างเข้าหากัน | `style.css`, `script.js` |
| `style.css` | ทำให้แผนที่เต็มหน้าจอ และให้ UI ลอยอยู่เหนือแผนที่ | `#map`, `.top-ui-container` ใน HTML |
| `script.js` | สร้างแผนที่จริง วาด Heatmap และแสดงข้อมูล IoT | `<div id="map">` ใน HTML |

---

## ⚠️ ปัญหาที่พบบ่อย

| อาการ | สาเหตุ | วิธีแก้ |
|---|---|---|
| แผนที่ไม่แสดง | `#map` ไม่มีความสูง | ตรวจสอบว่า `html, body` มี `height: 100%` |
| `L is not defined` | โหลด `script.js` ก่อน `leaflet.js` | เรียงลำดับ `<script>` ใหม่ให้ถูกต้อง |
| Heatmap ไม่แสดง | `leaflet-heat.js` โหลดก่อน `leaflet.js` | สลับลำดับให้ถูกต้อง |
| UI ถูกแผนที่บัง | `z-index` ของ `.top-ui-container` ต่ำเกินไป | เพิ่มค่าเป็น `z-index: 1000` ขึ้นไป |
| แผนที่แสดงครึ่งเดียว | Resize window หลังโหลดเสร็จ | เรียก `map.invalidateSize()` |

---

## Tech Stack

- [Leaflet.js](https://leafletjs.com/) `v1.9.4` — Interactive Map
- [Leaflet.heat](https://github.com/Leaflet/Leaflet.heat) — Heatmap Plugin
- [OpenStreetMap](https://www.openstreetmap.org/) — Map Tiles
- Vanilla HTML / CSS / JavaScript — ไม่ใช้ Framework
