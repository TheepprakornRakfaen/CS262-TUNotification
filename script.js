let map;
let heatLayer;


const LC2_LOCATION = [14.073587602014575, 100.60629940766313];

function initMap() {
    // 1. สร้างแผนที่และซูมเข้าไปที่ระดับอาคาร (Zoom 18)
    map = L.map('map', { 
        zoomControl: false 
    }).setView(LC2_LOCATION, 18);

    // 2. ใช้แผนที่จาก OpenStreetMap
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '© OpenStreetMap'
    }).addTo(map);

    // 3. แสดง Heatmap จากข้อมูล IoT
    renderHeatmap();

    // 4. มาร์กเกอร์ตำแหน่งปัจจุบัน (จำลอง)
    L.circleMarker(LC2_LOCATION, {
        radius: 8,
        fillColor: "#4285F4",
        color: "white",
        weight: 2,
        opacity: 1,
        fillOpacity: 1
    }).addTo(map).bindPopup("คุณอยู่ที่อาคาร LC2");
}

async function renderHeatmap() {
    const data = await fetchIoTData();

    // แปลงข้อมูลเป็น [lat, lng, intensity] 
    // โดยหารค่าเดซิเบลด้วย 150 เพื่อ Normalize ค่าให้อยู่ในช่วง 0-1
    const heatPoints = data.map(p => [p.latitude, p.longitude, p.decibel / 100]);

    // สร้าง Heatmap Layer
    heatLayer = L.heatLayer(heatPoints, {
        radius: 55, // รัศมีจุดความร้อน (ปรับให้ใหญ่พอดีกับขนาดตึก)
        blur: 20,   // ความฟุ้ง
        maxZoom: 18,
        gradient: {
            0.2: 'blue', 
            0.4: 'green', 
            0.6: 'yellow', 
            0.8: 'orange', 
            1.0: 'red'
        }
    }).addTo(map);
}

/// ข้อมูลจำลอง
async function fetchIoTData() {
    return [
        // กระจายจุดตามความยาวของตึก ซ้าย กลาง ขวา
        { "decibel": 45, "latitude": 14.073348245448962, "longitude": 100.60628465555105 }, 
        { "decibel": 70, "latitude":14.072630174271149, "longitude": 100.60631147764053}
    ];
}
// รันฟังก์ชันสร้างแผนที่
initMap();