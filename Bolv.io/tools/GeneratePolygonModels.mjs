// 平底の三角錐・十二面体と単色DDSを生成する。実行: node Bolv.io/tools/GeneratePolygonModels.mjs
// 頂点配置と当たり判定の半径は ShapeBox.cpp の値に合わせる。
import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
const output = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../Game/Assets/modelData/shape');
const specifications = [
    { name: 'ShapeTriangle', sides: 3, radius: 40, height: 60, color: [245, 80, 80] },
    { name: 'ShapePentagon', sides: 5, radius: 52, height: 60, color: [82, 130, 240] }
];
function dds(color) {
    const data = Buffer.alloc(132);
    data.write('DDS ', 0, 'ascii');
    const u32 = (offset, value) => data.writeUInt32LE(value >>> 0, offset);
    u32(4, 124); u32(8, 0x100f); u32(12, 1); u32(16, 1); u32(20, 4);
    u32(76, 32); u32(80, 0x41); u32(88, 32);
    u32(92, 0xff); u32(96, 0xff00); u32(100, 0xff0000); u32(104, 0xff000000);
    u32(108, 0x1000);
    data.set([...color, 255], 128);
    return data;
}
function model(spec) {
    const vertices = [], indices = [];
    const halfHeight = spec.height / 2;
    const ring = Array.from({length: spec.sides}, (_, i) => {
        const angle = -Math.PI / 2 + 2 * Math.PI * i / spec.sides;
        return [Math.cos(angle) * spec.radius, Math.sin(angle) * spec.radius];
    });
    function triangle(a, b, c, uv = [[0,0],[1,0],[0,1]]) {
        const ab = b.map((v,i) => v-a[i]), ac = c.map((v,i) => v-a[i]);
        const n = [ab[1]*ac[2]-ab[2]*ac[1], ab[2]*ac[0]-ab[0]*ac[2], ab[0]*ac[1]-ab[1]*ac[0]];
        const length = Math.hypot(...n);
        if (length < 0.0001) throw Error('Degenerate face');
        for (const [i,p] of [a,b,c].entries()) {
            vertices.push({p, n:n.map(v=>v/length), uv:uv[i]});
            indices.push(vertices.length); // TKMのインデックスは1始まり。
        }
    }
    if (spec.sides === 3) {
        // 底面が地面に接する三角錐。下面1枚、側面3枚で構成する。
        for (let i=0; i<3; ++i) {
            const a=ring[i], b=ring[(i+1)%3];
            triangle([0,halfHeight,0], [b[0],-halfHeight,b[1]], [a[0],-halfHeight,a[1]]);
        }
        triangle([ring[0][0],-halfHeight,ring[0][1]],
            [ring[1][0],-halfHeight,ring[1][1]], [ring[2][0],-halfHeight,ring[2][1]]);
    } else {
        // 正十二面体の20頂点から12枚の五角形を求める。面取りは行わない。
        const phi=(1+Math.sqrt(5))/2, points=[];
        for (const x of [-1,1]) for (const y of [-1,1]) for (const z of [-1,1]) points.push([x,y,z]);
        for (const a of [-1,1]) for (const b of [-1,1]) {
            points.push([0,a/phi,b*phi], [a/phi,b*phi,0], [b*phi,0,a/phi]);
        }
        const sub=(a,b)=>a.map((x,i)=>x-b[i]);
        const dot=(a,b)=>a.reduce((sum,x,i)=>sum+x*b[i],0);
        const cross=(a,b)=>[a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]];
        const unit=a=>a.map(x=>x/Math.hypot(...a));
        const faces=[], seen=new Set();
        for(let i=0;i<20;++i) for(let j=i+1;j<20;++j) for(let k=j+1;k<20;++k) {
            let normal=cross(sub(points[j],points[i]),sub(points[k],points[i]));
            if(Math.hypot(...normal)<1e-6) continue;
            normal=unit(normal);
            let d=dot(normal,points[i]);
            if(d<0){normal=normal.map(x=>-x);d=-d;}
            if(points.some(v=>dot(normal,v)>d+1e-6)) continue;
            const ids=points.map((v,id)=>Math.abs(dot(normal,v)-d)<1e-6?id:-1).filter(id=>id>=0);
            if(ids.length!==5) continue;
            const key=ids.join(',');
            if(!seen.has(key)){seen.add(key);faces.push({ids,normal});}
        }
        if(faces.length!==12) throw Error('Expected twelve pentagonal faces');
        // 一枚の五角形を真下に向け、底面全体が接地する姿勢にする。
        const up=faces[0].normal.map(x=>-x);
        const right=unit(cross(up,[0,0,1])), forward=cross(right,up);
        let oriented=points.map(v=>[dot(v,right),dot(v,up),dot(v,forward)]);
        const height=Math.max(...oriented.map(v=>Math.abs(v[1])));
        // 指定の高さと五角形の判定範囲内に収める。
        const maxRadius=Math.max(...oriented.map(v=>Math.hypot(v[0],v[2])));
        const horizontalRadius=spec.radius*Math.cos(Math.PI/5);
        oriented=oriented.map(v=>[v[0]*horizontalRadius/maxRadius,v[1]*halfHeight/height,v[2]*horizontalRadius/maxRadius]);
        for(const face of faces){
            const center=[0,0,0];
            for(const id of face.ids) for(let axis=0;axis<3;++axis) center[axis]+=oriented[id][axis]/5;
            let normal=unit(cross(sub(oriented[face.ids[1]],oriented[face.ids[0]]),sub(oriented[face.ids[2]],oriented[face.ids[0]])));
            if(dot(normal,center)<0) normal=normal.map(x=>-x);
            const u=unit(sub(oriented[face.ids[0]],center)),v=cross(normal,u);
            const ids=face.ids.slice().sort((a,b)=>{
                const pa=sub(oriented[a],center),pb=sub(oriented[b],center);
                return Math.atan2(dot(pa,v),dot(pa,u))-Math.atan2(dot(pb,v),dot(pb,u));
            });
            // 同一平面上の三角形に分割するだけなので、見た目は五角形の平面になる。
            for(let i=1;i<4;++i) triangle(oriented[ids[0]],oriented[ids[i]],oriented[ids[i+1]]);
        }
    }
    const parts=[];
    const header=Buffer.alloc(16);
    header.writeUInt8(100,0); header.writeUInt8(1,1); header.writeUInt16LE(1,2);
    header.writeUInt32LE(1,4); header.writeUInt32LE(vertices.length,8); header.writeUInt8(2,12);
    parts.push(header);
    for(const name of [spec.name+'_Albedo.DDS','','','','']) {
        const text=Buffer.from(name,'ascii'), length=Buffer.alloc(4);
        length.writeUInt32LE(text.length);
        parts.push(length);
        if(text.length) parts.push(text,Buffer.from([0]));
    }
    for(const v of vertices) {
        const data=Buffer.alloc(56);
        [...v.p,...v.n,...v.uv,0,0,0,0].forEach((x,i)=>data.writeFloatLE(x,i*4));
        // スキンなし。既存TKMと同じ未使用ボーン番号。
        for(let i=0;i<4;++i) data.writeInt16LE(-1,48+i*2);
        parts.push(data);
    }
    const ib=Buffer.alloc(4+indices.length*2);
    ib.writeUInt32LE(indices.length/3);
    indices.forEach((x,i)=>ib.writeUInt16LE(x,4+i*2));
    parts.push(ib);
    return Buffer.concat(parts);
}
await fs.mkdir(output,{recursive:true});
for(const spec of specifications) {
    await fs.writeFile(path.join(output,spec.name+'.tkm'),model(spec));
    await fs.writeFile(path.join(output,spec.name+'_Albedo.DDS'),dds(spec.color));
    console.log('Generated '+spec.name+' ('+spec.sides+' sides)');
}
