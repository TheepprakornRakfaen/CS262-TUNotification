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
    };
