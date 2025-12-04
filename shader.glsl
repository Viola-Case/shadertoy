// GLSL 4.5


int ITERATIONS = 500;

vec2 I(vec2 z, vec2 c) {
	return vec2(z.x*z.x-z.y*z.y,2*z.x*z.y) + c;
}

float M(vec2 z) {
	return 1.0/(z.x*z.x+z.y*z.y-4);
}

vec3 julia(vec2 z, vec2 c) {
	//if (dot(z,z) > 4.0) return vec3(0,0,0);
	float v = 0.1;
	for (int i = 0; i < ITERATIONS; ++i) {
		z = I(z,c);
		float r2 = dot(z,z);
		if (r2 > 4.0) {
			float smth = float(i) + 1.0 - log2(log2(r2));
			
			float h = 0.66667 - smth / float(ITERATIONS);
			return hsv2rgb(vec3(fract(h),1,v));
		}
		v = mix(v,1.0,clamp(9.0/float(2*ITERATIONS),0.0,0.8));
		//h += 1.0/float(ITERATIONS);
	}
	
	return vec3(0,0,0);
}

vec3 ss4julia(vec2 uv, vec2 c) {
    vec3 acc = vec3(0.0);
    vec2 px = 1.0 / uResolution;

    acc += julia(uv + vec2(-px.x, -px.y) * 0.5, c);
    acc += julia(uv + vec2( px.x, -px.y) * 0.5, c);
    acc += julia(uv + vec2(-px.x,  px.y) * 0.5, c);
    acc += julia(uv + vec2( px.x,  px.y) * 0.5, c);

    return acc * 0.25;
}

vec3 mandelbrot(vec2 uv) {
	return ss4julia(vec2(0,0),uv - vec2(0.5,0.));
}

void main() {
	
	
	vec2 uv = 2 * (fragCoord - vec2(0.5,0.5));
	
	vec2 muv = uMouse / uResolution - vec2(0.5,0.5);
	muv *= 2;
	
	
	
	vec3 col = mandelbrot(uv);
	//vec3 col = ss4julia(uv,muv);
	
	//vec3 col = vec3(0,1,1);
	
	//vec3 col = hsv2rgb(vec3(uTime*0.1,1.,1.));
	
	fragColor = vec4(col,1.);
}